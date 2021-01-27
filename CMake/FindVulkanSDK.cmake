# FindVulkanSDK.cmake
#
# Finds the Vulkan SDK
#
# This will define the following variables
#
#   VulkanSDK_FOUND
#   VulkanSDK_glslc_PROGRAM
#   VulkanSDK_glslangValidator_PROGRAM
#
# The following variables can be set as arguments
#
#   VulkanSDK_ROOT_DIR
#   ENV{VULKAN_SDK}
#

FIND_PROGRAM(
    VulkanSDK_glslc_PROGRAM
    glslc
    PATHS
        ${VulkanSDK_ROOT_DIR}
        $ENV{VULKAN_SDK}
    PATH_SUFFIXES
        bin
)

FIND_PROGRAM(
    VulkanSDK_glslangValidator_PROGRAM
    glslangValidator
    PATHS
        ${VulkanSDK_ROOT_DIR}
        $ENV{VULKAN_SDK}
    PATH_SUFFIXES
        bin
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    VulkanSDK
    HANDLE_COMPONENTS
    REQUIRED_VARS 
        VulkanSDK_glslc_PROGRAM
        VulkanSDK_glslangValidator_PROGRAM
)