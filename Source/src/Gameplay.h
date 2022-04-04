#pragma once

#if defined(HACHIKO_API)
// Do Nothing
#elif defined(_MSC_VER)
#define HACHIKO_API __declspec(dllexport)
#endif

namespace Time
{

HACHIKO_API float DeltaTime();

} // namespace Time