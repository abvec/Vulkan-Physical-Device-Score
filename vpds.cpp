/*
 *
 *
 */
#include "vpds.h"
#include <cstdio>
#include <unordered_map>
#include <string>

#ifdef VPDS_PRINT_ERROR
#define VPDS_ERROR(ERROR) fprintf(stderr,"%s\n",ERROR)
#else
#define VPDS_ERROR(ERROR)
#endif

#ifdef _MSC_VER
#include <intrin.h>
#define PLATFORM_POPCNT(X) __popcnt(X)
#define PLATFORM_LZCNT(X) _lzcnt_u32(X)
#elif defined __GNUC__
#define PLATFORM_POPCNT(X) __builtin_popcount(X)
#define PLATFORM_LZCNT(X) __builtin_lzcnt(X)
#else
#error "Unknown compiler!"
#endif

typedef struct VpdsScoreStructure_T {
    VpdsLookupCategoryBit   categories;
    uint32_t                scores[VPDS_LOOKUP_CATEGORY_LAST];
} * VpdsScoreStructure;

typedef struct VpdsScoreBoard_T {
    std::unordered_map<std::string,VpdsScoreStructure_T> deviceNameMap;
} * VpdsScoreBoard;

typedef struct VpdsScoreBoardInitializeInfo {
    uint8_t*    pExtend;
    uint64_t    extendSize;
} VpdsScoreBoardInitializeInfo;

void vpdsScoreBoradInsertInfo (VpdsScoreBoard board) {

    VpdsScoreBoard_T* const scoreBoard = (VpdsScoreBoard_T*)board;
    scoreBoard->deviceNameMap.reserve(200);

}

uint32_t vpdsCategoryToIndex (const VpdsLookupCategoryBit category) {
    return PLATFORM_LZCNT(category) + 1;
}

bool vpdsInitializeScoreBorad(const VpdsScoreBoard board, const VpdsScoreBoardInitializeInfo initializeInfo) {

    #ifndef NDEBUG

        if(board == nullptr) {
            VPDS_ERROR("vpdsInitializeScoreBorad() failed: board == nullptr");
            return false;
        }

    #endif

    vpdsScoreBoradInsertInfo(board);
    return true;
}

VpdsScoreStructure vpdsGetDeviceScoreStrucutre(VpdsScoreBoard board, const char* deviceName) {

    #ifndef NDEBUG

        if(board == nullptr) {
            VPDS_ERROR("vpdsGetDeviceScoreStrucutre() failed: board == nullptr");
            return false;
        }

        if(deviceName == nullptr) {
            VPDS_ERROR("vpdsGetDeviceScoreStrucutre() failed: deviceName == nullptr");
            return false;
        }

    #endif

    VpdsScoreBoard_T* const scoreBoard = (VpdsScoreBoard_T*)board;
    std::unordered_map<std::string,VpdsScoreStructure_T>::iterator iterator = scoreBoard->deviceNameMap.find(deviceName);
    if(iterator == scoreBoard->deviceNameMap.end()) {
        return nullptr;
    }

    return &iterator->second;
}

uint32_t vpdsReadScore(const VpdsScoreStructure structure, const uint32_t categoryIndex) {

    #ifndef NDEBUG

        if(structure == nullptr) {
            VPDS_ERROR("vpdsReadScore() failed: structure == nullptr");
            return 0;
        }

        if(categoryIndex > VPDS_LOOKUP_CATEGORY_LAST) {
            VPDS_ERROR("vpdsReadScore() failed: categoryIndex > VPDS_LOOKUP_CATEGORY_LAST");
            return 0;
        }

    #endif

    return structure->scores[categoryIndex];
}

VKAPI_ATTR VkResult VKAPI_CALL vpdsCreateScoreBoard(const VpdsScoreBoardCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VpdsScoreBoard* pBoard) {

    #ifndef NDEBUG

        if(pCreateInfo == nullptr) {
            VPDS_ERROR("vpdsCreateScoreBoard() failed: pCreateInfo == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(pCreateInfo->pNext != nullptr) {
            VPDS_ERROR("vpdsCreateScoreBoard() failed: pCreateInfo->pNext != nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(pAllocator != nullptr) {
            if(pAllocator->pfnAllocation == nullptr) {
                VPDS_ERROR("vpdsCreateScoreBoard() failed: pAllocator->pfnAllocation == nullptr");
                return VK_ERROR_INITIALIZATION_FAILED;
            }

            if(pAllocator->pfnFree == nullptr) {
                VPDS_ERROR("vpdsCreateScoreBoard() failed: pAllocator->pfnFree == nullptr");
                return VK_ERROR_INITIALIZATION_FAILED;
            }
        }

        if(pBoard == nullptr) {
            VPDS_ERROR("vpdsCreateScoreBoard() failed: pBoard == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

    #endif

    VpdsScoreBoard scoreBorad = VK_NULL_HANDLE;
    if(pAllocator == nullptr) {
        scoreBorad = new(std::nothrow) VpdsScoreBoard_T;
        if(scoreBorad == nullptr) {
            VPDS_ERROR("vpdsCreateScoreBoard() failed: (new(std::nothrow) VpdsScoreBoard_T) == nullptr)");
            return VK_ERROR_OUT_OF_HOST_MEMORY;
        }
    }
    else {
        scoreBorad = (VpdsScoreBoard)pAllocator->pfnAllocation(nullptr,sizeof(VpdsScoreBoard_T),alignof(VpdsScoreBoard_T),VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
        if(scoreBorad == nullptr) {
            VPDS_ERROR("vpdsCreateScoreBoard() failed: pAllocator->pfnAllocation(nullptr,sizeof(VpdsScoreBoard_T),alignof(VpdsScoreBoard_T),VK_SYSTEM_ALLOCATION_SCOPE_OBJECT) == nullptr");
            return VK_ERROR_OUT_OF_HOST_MEMORY;
        }
    }

    VpdsScoreBoardInitializeInfo initializeInfo;
    initializeInfo.pExtend = pCreateInfo->pExtend;
    initializeInfo.extendSize = pCreateInfo->extendSize;

    if(!vpdsInitializeScoreBorad(scoreBorad,initializeInfo)) {
        VPDS_ERROR("vpdsInitializeScoreBorad() failed: scoreBorad->initialize() == false");
        if(pAllocator == nullptr) {
            delete scoreBorad;
        }
        else {
            pAllocator->pfnFree(nullptr,scoreBorad);
        }

        return VK_ERROR_INITIALIZATION_FAILED;
    }

    pBoard = &scoreBorad;
    return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL vpdsGetPhysicalDeviceScore(const VpdsScoreBoard board, const VkPhysicalDevice device, const VpdsLookupInfo* pLookup, uint32_t* pScore) {

    #ifndef NDEBUG

        if(board == nullptr) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: board == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(device == nullptr) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: device == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(pLookup == nullptr) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: pLookup == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(pLookup->lookupCategory == 0) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: pLookup->lookupCategory == 0");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(pLookup->lookupCategory > VPDS_LOOKUP_CATEGORY_MAX) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: pLookup->lookupCategory > VPDS_LOOKUP_CATEGORY_MAX");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(pScore == nullptr) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: pScore == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

    #endif

    VkPhysicalDeviceProperties2 deviceProperties;
    deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;

    vkGetPhysicalDeviceProperties2(device,&deviceProperties);
    auto scoreStructure = vpdsGetDeviceScoreStrucutre(board,deviceProperties.properties.deviceName);
    if(scoreStructure == nullptr) {
        return VK_ERROR_UNKNOWN;
    }
    else {
        uint32_t writeIndex = 0;
        for(uint32_t readIndex = 0; readIndex < PLATFORM_POPCNT(VPDS_LOOKUP_CATEGORY_MAX); readIndex++) {
            if(pLookup->lookupCategory & (0x01f << readIndex)) {
                pScore[writeIndex] = vpdsReadScore(scoreStructure,readIndex);
                writeIndex = writeIndex + 1;
            }
        }
    }

    return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL vpdsDestroyScoreBoard(const VpdsScoreBoard board, const VkAllocationCallbacks* pAllocator) {

    #ifndef NDEBUG

        if(board == nullptr) {
            VPDS_ERROR("vpdsDestroyScoreBoard() failed: board == nullptr");
            return;
        }

    #endif

    if(pAllocator == nullptr) {
        delete board;
    }
    else {
        pAllocator->pfnFree(nullptr,board);
    }
}
