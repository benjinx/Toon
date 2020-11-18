#include <Temporality/Module.hpp>

#include <stdio.h>

namespace Temporality::STBI {

void ModuleInit() {
    printf("Init\n");
}

void ModuleTerm() {
    printf("Term\n");
}

TEMPORALITY_MODULE() {
    "STBI",
    ModuleInit,
    ModuleTerm,
};

} // namespace Temporality::STBI