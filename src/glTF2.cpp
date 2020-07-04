#include "glTF2.hpp"

//#include <Benchmark.hpp>
#include <Utils.hpp>
#include <nlohmann/json.hpp>
#include <Material.hpp>
//#include <Mesh.hpp>
#include <Texture.hpp>
//#include <GameObject.hpp>
#include <Camera.hpp>
#include <Log.hpp>
#include <glm/glm.hpp>
#include <MeshComponent.hpp>
#include <Light.hpp>

#include <cstdint>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include <stb/stb_image.h>

#include <Base64.hpp>

using namespace nlohmann;

namespace glTF2 {

	const uint32_t Magic = 0x46546C67; // glTF

	enum class ChunkType : uint32_t
	{
		JSON = 0x4E4F534A, // JSON
		BIN = 0x004E4942, // BIN
	};


	glm::vec3 parseVec3(const json& value, glm::vec3 def)
	{
		if (value.is_array() && value.size() == 3) {
			const auto& v = value.get<std::vector<float>>();
			return glm::make_vec3(v.data());
		}
		return def;
	}

	glm::vec4 parseVec4(const json& value, glm::vec4 def)
	{
		if (value.is_array() && value.size() == 4) {
			const auto& v = value.get<std::vector<float>>();
			return glm::make_vec4(v.data());
		}
		return def;
	}

	glm::quat parseQuat(const json& value, glm::quat def)
	{
		if (value.is_array() && value.size() == 4) {
			const auto& v = value.get<std::vector<float>>();
			return glm::quat(v[3], v[0], v[1], v[2]);
		}
		return def;
	}

	std::vector<std::vector<uint8_t>> loadBuffers(const json& data, const std::string& dir)
	{
		std::vector<std::vector<uint8_t>> buffers;

		auto it = data.find("buffers");
		if (it != data.end()) {
			if (it.value().is_array()) {
				auto& array = it.value();
				for (auto& object : array) {
					if (object.is_object()) {
						size_t byteLength = object.value<size_t>("byteLength", 0);
						const auto& uri = object.value("uri", "");

						if (uri.compare(0, strlen("data:"), "data:") == 0) {
							size_t pivot = uri.find(',');
							buffers.push_back(macaron::Base64::Decode(uri.substr(pivot + 1)));
						}
						else {
							LogVerbose("glTF buffer %zu, %s", byteLength, uri);

							std::ifstream bufferFile(dir + "/" + uri, std::ios::in | std::ios::binary);
							if (!bufferFile.is_open()) {
								LogError("Failed to open glTF data file '%s'", uri);
								continue;
							}

							buffers.push_back(std::vector<uint8_t>(byteLength));
							auto& buffer = buffers.back();

							bufferFile.read(reinterpret_cast<char *>(buffer.data()), byteLength);
							bufferFile.close();

							LogLoad("glTF data file '%s'", uri);

							if (buffer.size() != byteLength) {
								LogWarn("Buffer size mismatch %zu != %zu", buffer.size(), byteLength);
							}
						}
					}
				}
			}
		}

		return buffers;
	}

	struct bufferView_t {
		int buffer;
		size_t byteLength;
		size_t byteOffset;
		size_t byteStride;
		GLenum target;
	};

	std::vector<bufferView_t> loadBufferViews(const json& data)
	{
		std::vector<bufferView_t> bufferViews;

		const auto it = data.find("bufferViews");
		if (it != data.cend()) {
			if (it.value().is_array()) {
				const auto& array = it.value();
				for (const auto& object : array) {
					if (object.is_object()) {
						bufferViews.push_back(bufferView_t{
							object.value("buffer", -1),
							object.value<size_t>("byteLength", 0),
							object.value<size_t>("byteOffset", 0),
							object.value<size_t>("byteStride", 0),
							object.value<GLenum>("target", GL_INVALID_ENUM),
						});

						LogVerbose("BufferView %zu, %zu, %zu, %zu",
							bufferViews.back().buffer,
							bufferViews.back().byteLength,
							bufferViews.back().byteOffset,
							bufferViews.back().target);
					}
				}
			}
		}

		return bufferViews;
	}

	struct accessor_t {
		int bufferView;
		std::string type;
		size_t byteOffset;
		GLenum componentType;
		bool normalized;
		size_t count;
		// TODO: min, max
	};

	std::vector<accessor_t> loadAccessors(const json& data)
	{
		std::vector<accessor_t> accessors;

		const auto& it = data.find("accessors");
		if (it != data.cend()) {
			if (it.value().is_array()) {
				const auto& array = it.value();
				for (const auto& object : array) {
					if (object.is_object()) {
						accessors.push_back(accessor_t{
							object.value("bufferView", -1),
							object.value("type", ""),
							object.value<size_t>("byteOffset", 0),
							object.value<GLenum>("componentType", GL_INVALID_ENUM),
							object.value<bool>("normalized", false),
							object.value<size_t>("count", 0),
							// TODO: min, max
						});
					}
				}
			}
		}

		return accessors;
	}

	struct image_t {
		glm::ivec2 size;
		int components;
		std::unique_ptr<uint8_t> data;
	};

	std::vector<image_t> loadImages(
		const json& data,
		const std::string& dir,
		const std::vector<bufferView_t>& bufferViews,
		const std::vector<std::vector<uint8_t>>& buffers)
	{
		std::vector<image_t> images;

		const auto it = data.find("images");
		if (it != data.cend()) {
			if (it.value().is_array()) {
				const auto& array = it.value();
				for (const auto& object : array) {
					if (object.is_object()) {
						images.push_back(image_t{});
						auto& image = images.back();

						const auto& uri = object.value("uri", "");
						if (!uri.empty()) {
							const auto& imageFilename = dir + "/" + uri;
							image.data.reset(stbi_load(
								imageFilename.c_str(),
								&image.size.x,
								&image.size.y,
								&image.components,
								STBI_rgb_alpha)
								);
							image.components = STBI_rgb_alpha;

							if (!image.data) {
								LogError("Failed to load glTF image file '%s'", imageFilename);
								continue;
							}

							LogLoad("glTF image file '%s'", uri);
						}
						else {
							int bufferViewIndex = object.value("bufferView", -1);
							//const auto& mimeType = object.value("mimeType", "");

							const auto& bufferView = bufferViews[bufferViewIndex];
							const auto& buffer = buffers[bufferView.buffer];

                            stbi_set_flip_vertically_on_load(true);

							image.data.reset(stbi_load_from_memory(
								buffer.data() + bufferView.byteOffset,
								bufferView.byteLength,
								&image.size.x,
								&image.size.y,
								&image.components,
								STBI_rgb_alpha)
								);
							image.components = STBI_rgb_alpha;
						}
					}
				}
			}
		}

		return images;
	}

	std::vector<Texture::Options> loadSamplers(const json& data)
	{
		std::vector<Texture::Options> samplers;

		const auto it = data.find("samplers");
		if (it != data.cend()) {
			if (it.value().is_array()) {
				const auto& array = it.value();
				for (const auto& object : array) {
					if (object.is_object()) {
						samplers.push_back(Texture::Options{});
						auto& sampler = samplers.back();

						sampler.MagFilter = object.value<GLenum>("magFilter", sampler.MagFilter);
						sampler.MinFilter = object.value<GLenum>("minFilter", sampler.MinFilter);
						sampler.WrapS = object.value<GLenum>("wrapS", sampler.WrapS);
						sampler.WrapT = object.value<GLenum>("wrapT", sampler.WrapT);
					}
				}
			}
		}

		return samplers;
	}

	std::vector<std::shared_ptr<Texture>> loadTextures(
		const json& data,
		const std::vector<image_t>& images,
		const std::vector<Texture::Options> samplers)
	{
		std::vector<std::shared_ptr<Texture>> textures;

		const auto it = data.find("textures");
		if (it != data.cend()) {
			if (it.value().is_array()) {
				const auto& array = it.value();
				for (const auto& object : array) {
					if (object.is_object()) {
						int sampler = object.value("sampler", -1);
						int source = object.value("source", -1);

						if (source < 0) {
							LogError("Invalid glTF texture source %d", source);
							continue;
						}

						const auto& image = images[source];
						textures.push_back(std::make_shared<Texture>(
							image.data.get(),
							image.size,
							image.components,
							(sampler >= 0 ? samplers[sampler] : Texture::Options())
							));

						LogVerbose("Texture %zu, %zu", sampler, source);
					}
				}
			}
		}

		return textures;
	}

	struct camera_perspective_t {
	};

	struct camera_orthographic_t {
	};

	struct camera_t {
		std::string type;

		// Perspective
		float aspectRatio;
		float yfov;

		// Orthographic
		float xmag;
		float ymag;

		// Both
		float zfar;
		float znear;
	};

    struct light_t {
        glm::vec3 color;
        int intensity;
        std::string type;

        float innerConeAngle;
        float outerConeAngle;
    };

    std::vector<light_t> loadLights(const json& data)
    {
        std::vector<light_t> lights;
        const auto& lightData = data["extensions"]["KHR_lights_punctual"]["lights"];

        if (lightData.is_array())
        {
            for (const auto& it : lightData)
            {
                lights.push_back(light_t{});

                auto& light = lights.back();

                light.intensity = it.value("intensity", 1);
                light.type = it.value("type", "point");

                light.color = parseVec3(it.find("color").value(), glm::vec3(1));
                
                auto spot = it.find("spot");

                if (spot != it.end())
                {
                    light.innerConeAngle = spot.value().value("innerConeAngle", 0.1f);
                    light.outerConeAngle = spot.value().value("outerConeAngle", 0.1f);
                }
            }
        }

        return lights;
    }

	std::vector<camera_t> loadCameras(const json& data)
	{
		std::vector<camera_t> cameras;

		const auto& it = data.find("cameras");
		if (it != data.cend()) {
			if (it.value().is_array()) {
				const auto& array = it.value();
				for (const auto& object : array) {
					if (object.is_object()) {
						cameras.push_back(camera_t{
							object.value("type", ""),
						});
						auto& camera = cameras.back();

						if (camera.type == "perspective") {
							auto typeIt = object.find("perspective");
							if (typeIt != object.end()) {
								auto& perspective = typeIt.value();
								camera.aspectRatio = perspective.value("aspectRatio", 0.f);
								camera.yfov = perspective.value("yfov", 0.f);
								camera.zfar = perspective.value("zfar", 0.f);
								camera.znear = perspective.value("znear", 0.f);
							}
						}
						else if (camera.type == "orthographic") {
							auto typeIt = object.find("orthographic");
							if (typeIt != object.end()) {
								auto& orthographic = typeIt.value();
								camera.xmag = orthographic.value("xmag", 0.f);
								camera.ymag = orthographic.value("ymag", 0.f);
								camera.zfar = orthographic.value("zfar", 0.f);
								camera.znear = orthographic.value("znear", 0.f);
							}
						}
						else {
							LogWarn("Unknown camera type '%s'", camera.type);
						}
					}
				}
			}
		}

		return cameras;
	}

	std::vector<std::shared_ptr<Material>> loadMaterials(
		const json& data,
		const std::vector<std::shared_ptr<Texture>>& textures)
	{
		std::vector<std::shared_ptr<Material>> materials;

		auto parseTexture = [&textures](const json& value) -> std::shared_ptr < Texture > {
			if (value.is_object()) {
				int index = value.value("index", -1);
				int texCoord = value.value("texCoord", 0);

				if (index < 0 || index >(int)textures.size()) {
					LogError("Invalid glTF texture index %d", index);
					return nullptr;
				}

				if (texCoord > 0) {
					LogWarn("Multiple TEXCOORDs not supported");
				}

				return textures[index];
			}
			return nullptr;
		};

		const auto it = data.find("materials");
		if (it != data.cend()) {
			const auto& array = it.value();
			for (const auto& object : array) {
				if (object.is_object()) {
					materials.push_back(std::make_shared<Material>());
					auto& material = materials.back();

					LogVerbose("glTF material %s", object.value("name", ""));

					auto valIt = object.find("normalTexture");
					if (valIt != object.end()) {
						const auto& value = valIt.value();
                        material->SetMap(Material::NORMAL, parseTexture(value));
						if (value.is_object()) {
							material->SetNormalScale(value.value("scale", 1.0f));
						}
					}

					valIt = object.find("emissiveFactor");
					if (valIt != object.end()) {
					    material->SetEmissiveFactor(parseVec3(valIt.value(), material->GetEmissiveFactor()));
                    }

					valIt = object.find("emissiveTexture");
					if (valIt != object.end()) {
                        material->SetMap(Material::EMISSIVE, parseTexture(valIt.value()));
					}

					valIt = object.find("occlusionTexture");
					if (valIt != object.end()) {
						const auto& value = valIt.value();
                        material->SetMap(Material::AMBIENT_OCCLUSION, parseTexture(valIt.value()));
                        if (value.is_object()) {
                            material->SetOcclusionStrength(value.value("strength", 1.0f));
						}
					}

					const auto groupIt = object.find("pbrMetallicRoughness");
					if (groupIt != object.cend()) {
						const auto& group = groupIt.value();
						if (group.is_object()) {
							valIt = group.find("baseColorFactor");
							if (valIt != group.end()) {
                                material->SetBaseColorFactor(parseVec4(valIt.value(), material->GetBaseColorFactor()));
							}

							valIt = group.find("baseColorTexture");
							if (valIt != group.end()) {
                                material->SetMap(Material::BASE_COLOR, parseTexture(valIt.value()));
                                material->SetMap(Material::DIFFUSE, parseTexture(valIt.value()));
							}

							valIt = group.find("metallicFactor");
							if (valIt != group.end()) {
                                material->SetMetallicFactor(valIt.value().get<float>());
							}

							valIt = group.find("roughnessFactor");
							if (valIt != group.end()) {
								material->SetRoughnessFactor(valIt.value().get<float>());
							}

							valIt = group.find("metallicRoughnessTexture");
							if (valIt != group.end()) {
								const auto& value = valIt.value();
                                material->SetMap(Material::METALLIC_ROUGHNESS, parseTexture(value));
							}
						}
					}
				}
			}
		}

		return materials;
	}

	std::vector<Mesh::Primitive> loadPrimitives(
		const json& data,
		const std::vector<bufferView_t>& bufferViews,
		const std::vector<std::vector<uint8_t>>& buffers,
		const std::vector<accessor_t>& accessors,
		const std::vector<std::shared_ptr<Material>>& materials)
	{
		std::vector<Mesh::Primitive> primitives;

		std::shared_ptr<Material> defaultMaterial(new Material());

		std::vector<GLuint> vbos;

		const auto& primIt = data.find("primitives");
		if (primIt != data.end()) {
			const auto& primArray = primIt.value();
			if (primArray.is_array()) {
				for (const auto& primitive : primArray) {
					if (primitive.is_object()) {
						GLuint vao;
						glGenVertexArrays(1, &vao);
						glBindVertexArray(vao);

						int indices = primitive.value("indices", -1);
						if (indices < 0) {
							// TODO: glDrawArrays support
							LogError("glDrawArrays not supported");
							continue;
						}

						const auto& indexAccessor = accessors[indices];

						{
							const auto& bufferView = bufferViews[indexAccessor.bufferView];
							const auto& buffer = buffers[bufferView.buffer];

							GLenum target = (bufferView.target == GL_INVALID_ENUM ? GL_ELEMENT_ARRAY_BUFFER : bufferView.target);

							GLuint vbo;
							glGenBuffers(1, &vbo);
							vbos.push_back(vbo);

							glBindBuffer(target, vbo);
							glBufferData(target, bufferView.byteLength,
								buffer.data() + bufferView.byteOffset, GL_STATIC_DRAW);
						}

						const auto& attrIt = primitive.find("attributes");
						if (attrIt != primitive.end()) {
							const auto& attribs = attrIt.value();
							if (attribs.is_object()) {
								for (const auto&[attrib, accessorIndex] : attribs.items()) {
									auto& accessor = accessors[accessorIndex];
									auto& bufferView = bufferViews[accessor.bufferView];
									auto& buffer = buffers[bufferView.buffer];
									int byteStride = bufferView.byteStride;

									LogVerbose("glTF attribute %s", attrib);

									GLenum target = (bufferView.target == GL_INVALID_ENUM ? GL_ARRAY_BUFFER : bufferView.target);

									GLuint vbo;
									glGenBuffers(1, &vbo);
									vbos.push_back(vbo);

									glBindBuffer(target, vbo);
									glBufferData(target, bufferView.byteLength,
										buffer.data() + bufferView.byteOffset, GL_STATIC_DRAW);

									GLint size = -1;
									if (accessor.type == "SCALAR") {
										size = 1;
									}
									else if (accessor.type == "VEC2") {
										size = 2;
									}
									else if (accessor.type == "VEC3") {
										size = 3;
									}
									else if (accessor.type == "VEC4") {
										size = 4;
									}

									GLint vaa = -1;
									if (attrib == "POSITION") {
										vaa = Mesh::AttributeID::POSITION;
									}
									else if (attrib == "NORMAL") {
										vaa = Mesh::AttributeID::NORMAL;
									}
									else if (attrib == "TEXCOORD_0") {
										vaa = Mesh::AttributeID::TEXCOORD;
									}
									else if (attrib == "TANGENT") {
										vaa = Mesh::AttributeID::TANGENT;
									}

									if (vaa > -1) {
										glEnableVertexAttribArray(vaa);
										glVertexAttribPointer(
											vaa,
											size,
											accessor.componentType,
											accessor.normalized,
											byteStride,
											(void*)accessor.byteOffset
											);
									}
									else {
										LogWarn("Ignoring glTF attribute %s", attrib);
									}
								}
							}
						}

						int materialIndex = primitive.value("material", -1);

						std::shared_ptr<Material> material;
						if (materialIndex >= 0) {
							material = materials[materialIndex];
						}
						else {
							material = defaultMaterial;
						}

						LogVerbose("Primitive %u", vao);

						primitives.push_back({
							vao,
							primitive.value<GLenum>("mode", GL_TRIANGLES),
							(GLsizei)indexAccessor.count,
							(GLenum)indexAccessor.componentType,
							(GLsizei)indexAccessor.byteOffset,
							material,
						});
					}
				}
			}
		}

		return primitives;
	}

	std::vector<Mesh::Primitive> loadAllPrimitives(
		const json& data,
		const std::vector<bufferView_t>& bufferViews,
		const std::vector<std::vector<uint8_t>>& buffers,
		const std::vector<accessor_t>& accessors,
		const std::vector<std::shared_ptr<Material>>& materials)
	{
		std::vector<Mesh::Primitive> primitives;

		const auto it = data.find("meshes");
		if (it != data.cend()) {
			const auto& array = it.value();
			for (const auto& object : array) {
				if (object.is_object()) {
					LogVerbose("glTF mesh %s", object.value("name", ""));

					auto tmp = loadPrimitives(object, bufferViews, buffers, accessors, materials);
					for (auto&& p : tmp) {
						primitives.push_back(std::move(p));
					}
				}
			}
		}

		return primitives;
	}

	std::vector<std::shared_ptr<Mesh>> loadMeshes(
		const json& data,
		const std::vector<bufferView_t>& bufferViews,
		const std::vector<std::vector<uint8_t>>& buffers,
		const std::vector<accessor_t>& accessors,
		const std::vector<std::shared_ptr<Material>>& materials)
	{
		std::vector<std::shared_ptr<Mesh>> meshes;

		const auto it = data.find("meshes");
		if (it != data.cend()) {
			const auto& array = it.value();
			for (const auto& object : array) {
				if (object.is_object()) {
					LogVerbose("glTF mesh %s", object.value("name", ""));

					meshes.push_back(std::make_shared<Mesh>(
						loadPrimitives(object, bufferViews, buffers, accessors, materials)
						));
				}
			}
		}

		return meshes;
	}

	std::vector<std::unique_ptr<GameObject>> loadNodes(
		const json& data,
		const std::vector<camera_t>& cameras,
        const std::vector<light_t>& lights,
		const std::vector<std::shared_ptr<Mesh>>& meshes)
	{
		std::vector<std::unique_ptr<GameObject>> gobjs;

		std::function<std::unique_ptr<GameObject>(const json&, const json&)>
        loadNode = [&](const json& nodes, const json& data)
        -> std::unique_ptr<GameObject>
        {
            GameObject * gobj = nullptr;

			auto it = data.begin();

            int cameraIndex = data.value("camera", -1);

            auto lightPath = "/extensions/KHR_lights_punctual/light"_json_pointer;

            if (data.contains(lightPath))
            {
                auto lightIndexObject = data[lightPath];

                int lightIndex = lightIndexObject.get<int>();

                const auto& lightData = lights[lightIndex];

                Light * light = nullptr;

                if (lightData.type == "point")
                {
                    PointLight * pointLight = new PointLight();
                    light = (Light*)pointLight;
                }
                else if (lightData.type == "directional")
                {
                    DirectionalLight * directionalLight = new DirectionalLight();
                    light = (Light*)directionalLight;
                }
                else if (lightData.type == "spot")
                {
                    SpotLight * spotLight = new SpotLight();
                    light = (Light*)spotLight;
                    spotLight->SetCutOff(lightData.innerConeAngle);
                    spotLight->SetOuterCutOff(lightData.outerConeAngle);
                }

                light->SetIntensity(lightData.intensity);
                light->SetColor(lightData.color);

                gobj = light;
            }
            else if (cameraIndex >= 0)
            {
				Camera * camera = new Camera();
				const auto& c = cameras[cameraIndex];
				if (c.type == "perspective") {
					camera->SetMode(Camera::Mode::Perspective);

					// TODO: aspectRatio

					if (c.yfov > 0.f) {
						camera->SetFOVY(c.yfov);
					}

					if (c.znear > 0.f && c.zfar > 0.f) {
						camera->SetClip(c.znear, c.zfar);
					}
				}
				else if (c.type == "orthographic") {
					camera->SetMode(Camera::Mode::Orthographic);

					if (c.xmag > 0.f && c.ymag > 0.f) {
						camera->SetViewportSize(c.xmag, c.ymag);
					}

					if (c.znear > 0.f && c.zfar > 0.f) {
						camera->SetClip(c.znear, c.zfar);
					}
				}
				gobj = camera;
			}
			else {
				gobj = new GameObject();
			}

			int meshIndex = data.value("mesh", -1);
			if (meshIndex >= 0) {
				LogVerbose("Adding MeshComponent");
                gobj->AddComponent(std::make_unique<MeshComponent>(meshes[meshIndex]));
			}

			it = data.find("translation");
			if (it != data.end()) {
				gobj->SetPosition(parseVec3(it.value(), gobj->GetPosition()));
			}

			it = data.find("rotation");
			if (it != data.end()) {
				gobj->SetRotation(parseQuat(it.value(), gobj->GetRotation()));
			}

			it = data.find("scale");
			if (it != data.end()) {
				gobj->SetScale(parseVec3(it.value(), gobj->GetScale()));
			}

            it = data.find("children");
            if (it != data.end()) {
                for (const auto& child : it.value())
                {
                    gobj->AddChild(loadNode(nodes, nodes[child.get<int>()]));
                }
            }

			return std::unique_ptr<GameObject>(gobj);
		};

		std::vector<int> sceneNodeIndexes;

		int defaultSceneIndex = data.value("defaultScene", 0);
		const auto scenesIt = data.find("scenes");
		if (scenesIt != data.cend()) {
			const auto& array = scenesIt.value();
			if (array.is_array()) {
				const auto& scene = array[defaultSceneIndex];
				sceneNodeIndexes = scene["nodes"].get<std::vector<int>>();
			}
		}

		const auto nodesIt = data.find("nodes");
		if (nodesIt != data.cend()) {
			const auto& array = nodesIt.value();
			if (array.is_array()) {
				for (int index : sceneNodeIndexes) {
					const auto& object = array[index];

					if (object.is_object()) {
						LogVerbose("glTF node %s", object.value("name", ""));
						auto gobj = loadNode(array, object);
						if (gobj) {
							gobjs.push_back(std::move(gobj));
						}
					}
				}
			}
		}

		return gobjs;
	}

	std::tuple<json, std::vector<std::vector<uint8_t>>, std::string>
		loadFile(const std::string& filename)
	{
		static auto error = std::make_tuple(json(), std::vector<std::vector<uint8_t>>(), "");
		const auto& paths = Utils::GetResourcePaths();

		std::ifstream file;
		std::string fullPath;
		for (const auto& p : paths) {
			fullPath = p + filename;

            LogVerbose("Path: %s", p);
			LogVerbose("Checking %s", fullPath);

			file.open(fullPath.c_str(), std::ios::in | std::ios::binary);
			if (file.is_open()) {
				break;
			}
		}

		if (!file.is_open()) {
			LogError("Failed to load glTF, '%s'", filename);
			return error;
		}

		const auto& dir = Utils::GetDirname(fullPath);
		const auto& ext = Utils::GetExtension(filename);
		bool binary = (ext == "glb");

		std::vector<std::vector<uint8_t>> dataChunks;

		json data;
		if (binary) {
			uint32_t magic = 0;
			uint32_t version = 0;
			uint32_t length = 0;

			// TODO: Endianness

			file.read(reinterpret_cast<char *>(&magic), sizeof(magic));
			if (magic != Magic) {
				LogError("Invalid binary glTF file");
				return error;
			}

			file.read(reinterpret_cast<char *>(&version), sizeof(version));
			if (version != 2) {
				LogError("Invalid binary glTF container version %d", version);
				return error;
			}

			file.read(reinterpret_cast<char *>(&length), sizeof(length));

			uint32_t jsonChunkLength = 0;
			uint32_t jsonChunkType = 0;

			file.read(reinterpret_cast<char *>(&jsonChunkLength), sizeof(jsonChunkLength));
			file.read(reinterpret_cast<char *>(&jsonChunkType), sizeof(jsonChunkType));

			if ((ChunkType)jsonChunkType != ChunkType::JSON) {
				LogError("The first chunk of a binary glTF must be JSON, found %08x", jsonChunkType);
				return error;
			}

			std::vector<char> jsonChunk(jsonChunkLength + 1);
			file.read(jsonChunk.data(), jsonChunkLength);
			jsonChunk.back() = '\0';

			data = json::parse(jsonChunk.data());

			uint32_t dataChunkLength = 0;
			uint32_t dataChunkType = 0;
			while (file.read(reinterpret_cast<char *>(&dataChunkLength), sizeof(dataChunkLength))) {
				file.read(reinterpret_cast<char *>(&dataChunkType), sizeof(dataChunkType));

				if ((ChunkType)dataChunkType != ChunkType::BIN) {
					LogError("The second chunk of a binary glTF must be BIN, found %08x", dataChunkType);
					return error;
				}

				dataChunks.push_back(std::vector<uint8_t>(dataChunkLength));
				auto& dataChunk = dataChunks.back();

				file.read(reinterpret_cast<char *>(dataChunk.data()), dataChunkLength);
			}

		}
		else {
			file >> data;
		}

		if (auto it = data.find("asset"); it != data.end()) {
			const auto& object = it.value();
			if (object.is_object()) {

				const auto& version = object.value("version", "");

				LogVerbose("glTF Generator %s", object.value("generator", ""));
				LogVerbose("glTF Version %s", object.value("version", ""));

				if (version != "2.0") {
					LogError("only glTF 2.0 is supported");
					return error;
				}
			}
			else {
				LogError("glTF missing required asset entry");
				return error;
			}
		}

        // Should take both into some list then compare required to used and log an error if we don't have the right ones.

		if (auto it = data.find("extensionsRequired"); it != data.end()) {
			const auto& array = it.value();
			if (array.is_array()) {
				for (const auto& extension : array) {
					//LogError("Missing glTF required extension '%s'", extension);
                    LogInfo("glTF Required Extension found: %s", extension);
				}
			}
		}

		if (auto it = data.find("extensionsUsed"); it != data.end()) {
			const auto& array = it.value();
			if (array.is_array()) {
				for (const auto& extension : array) {
					//LogWarn("Missing glTF extension '%s'", extension);
                    LogInfo("glTF Extension found: %s", extension);
				}
			}
		}

        ////

		return std::make_tuple(data, dataChunks, dir);
	}

	std::vector<std::unique_ptr<GameObject>> LoadSceneFromFile(const std::string& filename)
	{
		//DuskBenchStart();

		const auto&[data, dataChunks, dir] = loadFile(filename);

		// TODO: Allow other buffers in GLB
		const auto& buffers = (dataChunks.empty() ? loadBuffers(data, dir) : dataChunks);
		const auto& bufferViews = loadBufferViews(data);
		const auto& accessors = loadAccessors(data);
		const auto& images = loadImages(data, dir, bufferViews, buffers);
		const auto& samplers = loadSamplers(data);
		const auto& textures = loadTextures(data, images, samplers);
		const auto& cameras = loadCameras(data);
		const auto& materials = loadMaterials(data, textures);
		const auto& meshes = loadMeshes(data, bufferViews, buffers, accessors, materials);
        const auto& lights = loadLights(data);
		auto gobjs = loadNodes(data, cameras, lights, meshes);

		//DuskBenchEnd("glTF2::LoadSceneFromFile");
		return gobjs;
	}

	std::vector<Mesh::Primitive> LoadPrimitivesFromFile(const std::string& filename)
	{
		//DuskBenchStart();

		const auto&[data, dataChunks, dir] = loadFile(filename);

		// TODO: Allow other buffers in GLB
		const auto& buffers = (dataChunks.empty() ? loadBuffers(data, dir) : dataChunks);
		const auto& bufferViews = loadBufferViews(data);
		const auto& accessors = loadAccessors(data);
		const auto& images = loadImages(data, dir, bufferViews, buffers);
		const auto& samplers = loadSamplers(data);
		const auto& textures = loadTextures(data, images, samplers);
		const auto& materials = loadMaterials(data, textures);
		auto primitives = loadAllPrimitives(data, bufferViews, buffers, accessors, materials);

		//DuskBenchEnd("glTF2::LoadMeshFromFile");
		return primitives;
	}

} // namespace glTF2