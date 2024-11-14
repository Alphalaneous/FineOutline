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
