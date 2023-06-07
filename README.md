# ScreenshotBOFPlus

 Take a screenshot without injection for Cobalt Strike. I only made minor optimizations to the existing code, and made it support the ability to get a complete screenshot when global scaling is initiated on Windows.

## Self Compilation
1. git clone the repo
2. open the solution in Visual Studio
3. Build project BOF

## Usage
1. import the ScreenshotBOFPlus.cna script into Cobalt Strike
2. use the command screenshot_plus
  
```
beacon> screenshot_plus
[*] Running screenshot without injection
...
```

3. if take sceenshot successfully, you should find a screenshot on View-Screenshots
  
## Credits
- https://github.com/CodeXTF2/ScreenshotBOF
- https://github.com/qwqdanchun/ScreenShot-BOF

## Disclaimer
usual disclaimer here, I am not responsible for any crimes against humanity you may commit or nuclear war you may cause using this piece of poorly written code.
