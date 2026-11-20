/*
 *
 *
 */
#include "vpds.h"

#include <cstdio>

#ifdef VPDS_PRINT_ERROR
#define VPDS_ERROR(ERROR) fprintf(stderr,"%s\n",ERROR)
#else
#define VPDS_ERROR(ERROR)
#endif

typedef struct VpdsScoreStructure_T {


} * VpdsScoreStructure;

typedef struct VpdsScoreBoard {


} * VpdsScoreBoard;

typedef struct VpdsScoreBoardInitializeInfo {
    uint8_t*    pExtend;
    uint64_t    extendSize;
} VpdsScoreBoardInitializeInfo;

bool vpdsInitializeScoreBorad(const VpdsScoreBoard board, const VpdsScoreBoardInitializeInfo initializeInfo) {

    return true;
}

VpdsScoreStructure vpdsGetDeviceScoreStrucutre(const VpdsScoreBoard board, const char* deviceName) {


}

void vpdsReadScore(const VpdsScoreStructure structure, const VpdsLookupCategory category) {

}

/*  */
VKAPI_ATTR VkResult VKAPI_CALL vpdsCreateScoreBoard(const VpdsScoreBoardCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VpdsScoreBoard* pBoard) {

    #ifndef NDEBUG

        if(pCreateInfo == nullptr) {
            VPDS_ERROR("vpdsCreateScoreBoard() failed: pCreateInfo == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(pCreateInfo->sType != VPDS_STRUCTURE_TYPE_SCORE_BOARD_CREATE_INFO) {
            VPDS_ERROR("vpdsCreateScoreBoard() failed: pCreateInfo->sType != VPDS_STRUCTURE_TYPE_SCORE_BOARD_CREATE_INFO");
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
        scoreBorad = pAllocator->pfnAllocation(nullptr,sizeof(VpdsScoreBoard_T),alignof(VpdsScoreBoard_T),VK_SYSTEM_ALLOCATION_SCOPE_OBJECT);
        if(scoreBorad == nullptr) {
            VPDS_ERROR("vpdsCreateScoreBoard() failed: pAllocator->pfnAllocation(nullptr,sizeof(VpdsScoreBoard_T),alignof(VpdsScoreBoard_T),VK_SYSTEM_ALLOCATION_SCOPE_OBJECT) == nullptr");
            return VK_ERROR_OUT_OF_HOST_MEMORY;
        }
    }

    VpdsScoreBoardInitializeInfo initializeInfo {
        .pExtend = pCreateInfo->pExtend,
        .extendSize = pCreateInfo->extendSize,
    };

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

/*  */
VKAPI_ATTR VkResult VKAPI_CALL vpdsGetPhysicalDeviceScore(const VpFunctions functions, const VkPhysicalDevice device, const VpdsScoreBoard board, const VpdsLookupInfo* pLookup, const uint32_t* pScore) {

    #ifndef NDEBUG

        if(functions == nullptr) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: functions == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(functions->GetPhysicalDeviceProperties2 == nullptr) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: functions->GetPhysicalDeviceProperties2 == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(device == nullptr) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: device == nullptr");
            return VK_ERROR_INITIALIZATION_FAILED;
        }

        if(board == nullptr) {
            VPDS_ERROR("vpdsGetPhysicalDeviceScore() failed: board == nullptr");
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

    VkPhysicalDeviceProperties2 deviceProperties {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2
    };

    functions->GetPhysicalDeviceProperties2(device,&device_properties);
    const VpdsScoreBoard_T* scoreBoard = (VpdsScoreBoard_T*)board;

    auto scoreStructure = vpdsGetDeviceScoreStrucutre(scoreBoard,deviceProperties.properties.deviceName);
    if(scoreStructure == nullptr) {
        return VK_ERROR_UNKNOWN;
    }
    else {
        uint32_t writeOffset = 0;
        for(uint32_t readOffset = 0; readOffset < popcnt(VPDS_LOOKUP_CATEGORY_MAX); readOffset++) {
            if(pLookup->lookupCategory & (0x01f << readOffset)) {
                pScore[writeOffset] = vpdsReadScore(scoreStructure,readOffset);
                writeOffset = writeOffset + 1;
            }
        }
    }

    return VK_SUCCESS;
}

/*  */
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
