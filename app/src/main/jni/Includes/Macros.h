// thanks to shmoo and joeyjurjens for the usefull stuff under this comment.
#ifndef ANDROID_MOD_MENU_MACROS_H
#define ANDROID_MOD_MENU_MACROS_H

#include <Dobby/dobby.h>

void hook(void *offset, void* ptr, void **orig)
{
    DobbyHook(offset, ptr, orig);
}

#define HOOK_LIB(lib, offset, ptr, orig) hook((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, (void **)&orig)
#define HOOK_LIB_NO_ORIG(lib, offset, ptr) hook((void *)getAbsoluteAddress(OBFUSCATE(lib), string2Offset(OBFUSCATE(offset))), (void *)ptr, NULL)
#define HOOKSYM_LIB(lib, sym, ptr, org) hook(dlsym(dlopen(OBFUSCATE(lib), 4), OBFUSCATE(sym)), (void *)ptr, (void **)&org)
#define HOOKSYM_LIB_NO_ORIG(lib, sym, ptr) hook(dlsym(dlopen(OBFUSCATE(lib), 4), OBFUSCATE(sym)), (void *)ptr, NULL)

#endif //ANDROID_MOD_MENU_MACROS_H
