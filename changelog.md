# 1.4.0
- Show other people's outline colors
- Show outline colors in GJLevelScoreCell, GJRequestCell, and GJUserCell
- Bug fixes

# 1.3.3
- Fixed fine outline affecting icons without an outline color set

# 1.3.2
- Tweaked shaders even more
  - Less washed out for real this time
- Fix a bug where custom colors wouldn't update visually in the Icon Kit
- Fix a bug where the default outline color was green

# 1.3.1
- Tweaked shaders even more
  - Less washed out
  - Less gaps

# 1.3.0
- Added Optional API
- Tweaked shaders some more
- Code cleanup

# 1.2.5
- More shader tweaks
  - Removed UFO dome coloring setting due to internal changes

# 1.2.4
- Fix UFO dome being colored
  - Add a setting for it cuz it looks cool

# 1.2.3
- RobTop's GJRobotSprite and GJSpiderSprite are cursed
  - Fixed a crash (I hope)

# 1.2.2
- Fix a crash

# 1.2.1
- Code cleanup
- Separate Dual Icons
  - Allows for the potential for a future API
  - No more needing to work around colors being set to the overlay sprite
  - Easier to scale if more players exist in the future for any reason
  - Bug fixes (It now works properly again!)
  - Support Separate Dual Icons swap button
  - Fix crash with profile 2P button after refreshing 
- Fix Player 2 sometimes not having an outline color set
- Remove Alt Shader, main one should work for all cases now
- Fix choosing a black outline not properly updating the player

# 1.2.0
- Better isolation shaders
  - No more thin outline still being visible
- 2.2081 support

# 1.1.8
- One more fix

# 1.1.7
- Fix typo

# 1.1.6
- Code cleanup and compatibility fixes

# 1.1.5
- Fix a crash with More Icons

# 1.1.4
- Less invasive code

# 1.1.2 - 1.1.3
- iOS support and fixes

# 1.1.1
- Fix issue with Custom Gamemode Colors
- Fix issue where default black outline wasn't being set properly
- *Maybe* fix issue where More Icons icons use the wrong outline

# 1.1.0
- Revamp custom line colors to have a separate color tab
- Have optional separate color isolator to try if colors look weird on your icon

# 1.0.10/11
- 2.2074 support

# 1.0.9
- Fix a typo in the description

# 1.0.8
- Changes by [@hiimjustin000](https://github.com/hiimjustin000)
    - MacOS support
    - Separate Dual Icons support
    - Fix reloading shaders (no more wonky icons on texture reload)

# 1.0.7
- I guess don't try anything, doing anything ever will crash on android

# 1.0.6
- Revert 1.0.5 due to issues with android.
- Remove hacky More Icons fix now that More Icons properly calls onSelect

# 1.0.5
- Made color bleeding less apparent.

# 1.0.4
- Make outline opacity less convoluted.

# 1.0.3
- Fix outline not showing on your icon on comments and leaderboards.
- Fix bug with xdbot respawn flash.

# 1.0.2
- Fix z ordering of outline

# 1.0.1
- Tweak the shader's extraction algorithm to not catch normal colors as often.
- Setting the color to black disables any icon shaders rather than leaving the custom ones with no visual effect (useful for colored icons, as previously it would still affect those).
- Fixed a bug where dying in the editor removed the outline.
- Fixed a bug where icons selected using More Icons would show the default icon outline.

# 1.0.0
- Initial Release.
