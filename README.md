# PDFSolid Conversion SDK for C (Windows)

High-performance C SDK for converting PDF to Word, Excel, PowerPoint, HTML, Image, TXT, RTF, CSV, JSON, Markdown, Searchable PDF, and OFD with AI-powered OCR, layout analysis, and table recognition.

## Features

- **PDF to Word** (.docx) — Flow and Box layout modes
- **PDF to Excel** (.xlsx) — per-table, per-page, or per-document worksheet options
- **PDF to PowerPoint** (.pptx)
- **PDF to HTML** (.html) — single/multi-page with optional bookmark navigation
- **PDF to CSV** (.csv)
- **PDF to Image** (.png, .jpg, .jpeg, .jpeg2000, .bmp, .tiff, .tga, .gif, .webp) — color/grayscale/binary, configurable scaling
- **PDF to Plain Text** (.txt) — optional table format preservation
- **PDF to RTF** (.rtf)
- **PDF to Searchable PDF** (.pdf) — OCR with transparent text layer
- **PDF to OFD** (.ofd) — OCR, page background preservation, transparent text layer
- **PDF to JSON** (.json) — structured data with table extraction
- **PDF to Markdown** (.md)

### AI-Powered Document Tools

- **OCR** — Optical Character Recognition for scanned documents and images
- **Layout Analysis** — AI-based document structure parsing
- **Table Recognition** — AI-based table structure reconstruction
- **Custom AI Models** — plug in your own OCR, layout, or table engine via callbacks (SDK v1.1.0+)

## Requirements

| Platform | System Requirements | Development Environment |
| -------- | ------------------- | ----------------------- |
| Windows | Windows 7, 8, 10, 11 (x86, x64) | Visual Studio 2022+ |

## Quick Start

### 1. Get a License

Contact [sales@pdfsolid.com](mailto:sales@pdfsolid.com) for a 30-day free trial or commercial license.

### 2. Apply License and Initialize

```c
#include "pdfsolid_common_c.h"

#if defined(_WIN32)
#define CPDF_TEXT(text) L##text
#else
#define CPDF_TEXT(text) text
#endif

CSDKErrorCode code = CPDF_LicenseVerify(CPDF_TEXT("LICENSE_KEY"), CPDF_TEXT("device_id"), CPDF_TEXT("app_id"));
if (code != e_CErrSuccess) {
    return code;
}

CPDF_Initialize(CPDF_TEXT("PDFSolid_Conversion_SDK/resource"));
```

### 3. Convert

```c
#include "pdfsolid_conversion_c.h"

CConvertOption option = CPDF_CONVERT_OPTION_DEFAULT;
CPDF_StartPDFToWord(CPDF_TEXT("input.pdf"), CPDF_TEXT(""), CPDF_TEXT("output.docx"), option, NULL);
```

### Release Resources

```c
CPDF_Release();
```

## Conversion Examples

### PDF to Excel

```c
CConvertOption option = CPDF_CONVERT_OPTION_DEFAULT;
option.excel_worksheet_option = e_CForTable;
CPDF_StartPDFToExcel(CPDF_TEXT("input.pdf"), CPDF_TEXT(""), CPDF_TEXT("output.xlsx"), option, NULL);
```

### PDF to Image

```c
CConvertOption option = CPDF_CONVERT_OPTION_DEFAULT;
option.image_type = e_CPNG;
option.image_scaling = 2.0f;
CPDF_StartPDFToImage(CPDF_TEXT("input.pdf"), CPDF_TEXT(""), CPDF_TEXT("output"), option, NULL);
```

### PDF to Searchable PDF (OCR)

```c
CPDF_SetDocumentAIModel(CPDF_TEXT("path/model"));

CConvertOption option = CPDF_CONVERT_OPTION_DEFAULT;
option.enable_ocr = true;
COCRLanguage languages[] = { e_CENGLISH };
option.languages = languages;
option.language_count = 1;
option.transparent_text = true;
CPDF_StartPDFToSearchablePDF(CPDF_TEXT("scan.pdf"), CPDF_TEXT(""), CPDF_TEXT("output.pdf"), option, NULL);
```

### PDF to JSON with Table Extraction

```c
CConvertOption option = CPDF_CONVERT_OPTION_DEFAULT;
option.json_contain_table = true;
CPDF_StartPDFToJson(CPDF_TEXT("input.pdf"), CPDF_TEXT(""), CPDF_TEXT("output.json"), option, NULL);
```

### Custom AI Engine (SDK v1.1.0+)

```c
static bool my_ocr(const wchar_t *image_path) { /* your OCR */ return true; }
static const wchar_t *my_ocr_result(void) { return L""; }
static bool my_layout(const wchar_t *image_path) { /* your layout */ return true; }
static const wchar_t *my_layout_result(void) { return L""; }
static bool my_table(const wchar_t *image_path) { /* your table */ return true; }
static const wchar_t *my_table_result(void) { return L""; }

CConvertCallback callback = {0};
callback.ocr = my_ocr;
callback.get_ocr_result = my_ocr_result;
callback.layout = my_layout;
callback.get_layout_result = my_layout_result;
callback.table = my_table;
callback.get_table_result = my_table_result;

CConvertOption option = CPDF_CONVERT_OPTION_DEFAULT;
option.enable_ocr = true;
option.enable_ai_layout = true;
CPDF_StartPDFToWord(CPDF_TEXT("input.pdf"), CPDF_TEXT(""), CPDF_TEXT("output.docx"), option, &callback);
```

## Documentation

- [Developer Guide](doc/developer_guide_c.md)
- [API Reference](doc/api_reference_c.html)

## Contact

- Website: [https://www.pdfsolid.com](https://www.pdfsolid.com/)
- Sales: [sales@pdfsolid.com](mailto:sales@pdfsolid.com)
- Support: [support@pdfsolid.com](mailto:support@pdfsolid.com)
