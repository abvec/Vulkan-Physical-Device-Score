/*
 *
 *
 */
#ifndef VPDS_H
#define VPDS_H

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_profiles.h>

#define VPDS_PRINT_ERROR

/*  */
typedef struct VpdsScoreBoardCreateInfo {
    /*  */
    VkStructureType sType;
    /*  */
    const void* pNext;
    /*  */
    uint8_t*    pExtend;
    /*  */
    uint64_t    extendSize;
} VpdsScoreBoardCreateInfo;

/*  */
typedef enum VpdsLookupCategoryBit {
    VPDS_LOOKUP_CATEGORY_GRAPHICS_BIT = 0x01,
    VPDS_LOOKUP_CATEGORY_COMPUTE_BIT = 0x02,
    VPDS_LOOKUP_CATEGORY_MAX = 0x03,
} VpdsScoreLookupBit;

/*  */
typedef VkFlags VpdsLookupCategory;
/*  */
typedef struct VpdsLookupInfo {
    /*  */
    VkStructureType     sType;
    /*  */
    const void*         pNext;
    /*  */
    VpdsLookupCategory  lookupCategory;
} VpdsLookupDescription;

/*  */
VK_DEFINE_HANDLE(VpdsScoreBoard);

/*  */
VKAPI_ATTR VkResult VKAPI_CALL vpdsCreateScoreBoard(const VpdsScoreBoardCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VpdsScoreBoard* pBoard);
/*  */
VKAPI_ATTR VkResult VKAPI_CALL vpdsGetPhysicalDeviceScore(const VpFunctions functions, const VkPhysicalDevice device, const VpdsScoreBoard board, const VpdsLookupInfo* pLookup, const uint32_t* pScore);
/*  */
VKAPI_ATTR void VKAPI_CALL vpdsDestroyScoreBoard(const VpdsScoreBoard board, const VkAllocationCallbacks* pAllocator);

#endif
