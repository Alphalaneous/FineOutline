# Fine Outline

## Change the outline color of icons! 

Many years ago, <cl>RobTop</c> teased a potential new feature that would allow you to customize the outline color of icons. This never made it into the game, and thus we were stuck with a black outline. This mod implements that feature in a way that works across the board with *any* icon. 

This mod uses a shader to isolate the outline from an icon and make it colorable. This means it will work even with icons from a texture pack. Not all icons are the same, and if there are stuff like straight up images being used as an icon, it might not look good, but that's on you.

Using this mod is easy. Simply go to the <cy>**color palette**</c> button in the icon kit, and click on the <cg>Line</c> tab on the top right. From there you can choose any default color, or a custom one.

# Optional API

Mod developers can set the colors for the icon or change around the player's icon settings if needed. 

`#include <alphalaneous.fine_outline/include/FineOutline.hpp>`

These are all under the `alpha::fine_outline` namespace.

```cpp
/// Set the outline color of a PlayerObject or SimplePlayer
void setOutlineColor(T* player, const cocos2d::ccColor3B& color)

/// Get the outline color of a PlayerObject or SimplePlayer
cocos2d::ccColor3B getOutlineColor(T* player)

/// Update the outline color of a PlayerObject or SimplePlayer
/// This is useful if something else changed it for some reason
void updateOutline(T* player)

/// Check if the 2 player icons are separate with the "Separate Dual Icons" mod
bool isSeparate()

/// Check if the second player is being edited in the Garage with the "Separate Dual Icons" mod
bool isEditingP2()

/// Returns true if the player icon type is using a custom color rather than a color index
/// By default, it gets the selected icon's color if Separate dual icons is installed
bool usesCustomColor(alpha::fine_outline::PlayerIcon player)

/// Set if the player icon type is using a custom color rather than a color index
void useCustomColor(alpha::fine_outline::PlayerIcon player, bool use)

/// Returns the custom color set for a player icon type
/// By default, it gets the selected icon's color if Separate dual icons is installed
cocos2d::ccColor3B getCustomColor(alpha::fine_outline::PlayerIcon player)

/// Sets the custom color for a player icon type
void setCustomColor(alpha::fine_outline::PlayerIcon player, const cocos2d::ccColor3B& color)

/// Gets the color index for a player icon type
/// By default, it gets the selected icon's color if Separate dual icons is installed
int getColorIndex(alpha::fine_outline::PlayerIcon player)

/// Sets the color index for a player icon type
void setColorIndex(alpha::fine_outline::PlayerIcon player, int color)

/// Gets the color for a player icon type (returns the custom one if custom, else returns the color for color index)
/// By default, it gets the selected icon's color if Separate dual icons is installed
cocos2d::ccColor3B getColor(alpha::fine_outline::PlayerIcon player)
```