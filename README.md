# Vulkan-Physical-Device-Score
Allows users to rate Vulkan physical devices based on their performance

# How to use it:

1 Create `VpdsScoreBoard`
```c++
VpdsScoreBoard score_board {VK_NULL_HANDLE};
VpdsScoreBoardCreateInfo score_board_create_info {}; // placeholder
if(vpdsCreateScoreBoard(&score_board_create_info,nullptr,&score_board) != VK_SUCCESS) {
	// handle error
}
```

2 Initialize `VpdsLookupInfo`
```c++
VpdsLookupInfo lookup_info {
	.lookupCategory = VPDS_LOOKUP_CATEGORY_GRAPHICS_BIT,
};
```

3 Read the score
```c++
uint32_t score = 0;
if(vpdsGetPhysicalDeviceScore(score_board,physical_device_handle,lookup_info,&score) != VK_SUCCESS) {
	// could not find score, call your own fallback function
	score = fallback_score_function(...);
}
```

4 Destroy `VpdsScoreBoard`
```c++
vpdsDestroyScoreBoard(score_board,nullptr);
```

Looking up multiple categories at once
```c++
VpdsLookupInfo lookup_info {
	// look up graphics and compute scores
	.lookupCategory = VPDS_LOOKUP_CATEGORY_GRAPHICS_BIT | VPDS_LOOKUP_CATEGORY_COMPUTE_BIT,
	// combine those scores by adding them up
	.combineFunc = VPDS_COMBINE_ADD,
};

uint32_t score = 0;
if(vpdsGetPhysicalDeviceScore(score_board,physical_device_handle,lookup_info,&score) != VK_SUCCESS) {
	// could not find score, call your own fallback function
	score = fallback_score_function(...);
}
```
