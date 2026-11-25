#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "pdfsolid_common_c.h"
#include "pdfsolid_conversion_c.h"

// A license key string, or the path of a license.xml file.
static const char* LICENSE = "..\\license.xml";
static const wchar_t* PASSWORD = L"";

static void progress_callback(int current_page_count, int total_page_count)
{
    printf("convert rate: %d/%d\n", current_page_count, total_page_count);
}

static bool cancel_callback()
{
    // Return true to cancel the conversion, false to continue.
    return false;
}

static void print_result(const char* name, CSDKErrorCode error_code)
{
    printf("%s: %d\n", name, (int)error_code);
}

static void run_word_to_pdf(CConvertCallback* callback)
{
    // Word to PDF reads only page_ranges and word_pdf_bookmark_mode; other fields are ignored.
    CConvertOption word_options = CPDF_CONVERT_OPTION_DEFAULT;
    char diagnostics[CPDF_WORD_TO_PDF_DIAGNOSTICS_SIZE] = { 0 };
    CSDKErrorCode error_code;

    // word to pdf with default options
    print_result("word to pdf",
                 CPDF_StartWordToPDF(L"..\\input_files\\word.docx",
                                     L"..\\output_files\\word_to_pdf.pdf",
                                     word_options, callback, NULL));

    // word to pdf with heading bookmarks, a page range and diagnostics
    word_options.word_pdf_bookmark_mode = e_CWordPdfBookmarkHeadings;
    strncpy_s(word_options.page_ranges, sizeof(word_options.page_ranges), "1-2", _TRUNCATE);
    error_code = CPDF_StartWordToPDF(L"..\\input_files\\word.docx",
                                     L"..\\output_files\\word_to_pdf_bookmarks.pdf",
                                     word_options, callback, diagnostics);
    print_result("word to pdf with options", error_code);
    // e_CErrUnsupportedFeature means the PDF is usually still produced, but some
    // features may affect fidelity; the diagnostics text lists them.
    if (diagnostics[0] != '\0') {
        printf("word to pdf diagnostics:\n%s\n", diagnostics);
    }
}

int main(void)
{
    CConvertOption options = CPDF_CONVERT_OPTION_DEFAULT;
    CConvertCallback callback;
    COCRLanguage languages[] = { e_CENGLISH };
    char version[30] = { 0 };

    // Zero-initialize so the optional AI hooks (ocr, layout, table, ...) stay null.
    memset(&callback, 0, sizeof(callback));
    callback.progress = progress_callback;
    callback.cancel = cancel_callback;

    print_result("pdfsolid conversion sdk license verify",
                 CPDF_LicenseVerify(LICENSE, "", ""));
    CPDF_Initialize(L"..\\..\\resource");
    CPDF_SetLogger(false, false);
    print_result("set document ai model",
                 CPDF_SetDocumentAIModel(L"..\\..\\resource\\models\\documentai.model", -1));

    print_result("pdf to word",
                 CPDF_StartPDFToWord(L"..\\input_files\\word.pdf", PASSWORD,
                                     L"..\\output_files\\word.docx", options, &callback));
    print_result("pdf to excel",
                 CPDF_StartPDFToExcel(L"..\\input_files\\excel.pdf", PASSWORD,
                                      L"..\\output_files\\excel.xlsx", options, &callback));
    print_result("pdf to ppt",
                 CPDF_StartPDFToPpt(L"..\\input_files\\powerpoint.pdf", PASSWORD,
                                    L"..\\output_files\\powerpoint.pptx", options, &callback));

    options.excel_csv_format = true;
    print_result("pdf to csv",
                 CPDF_StartPDFToExcel(L"..\\input_files\\excel.pdf", PASSWORD,
                                      L"..\\output_files\\", options, &callback));
    options.excel_csv_format = false;

    options.page_layout_mode = e_PageLayoutModeBox;
    print_result("pdf to html",
                 CPDF_StartPDFToHtml(L"..\\input_files\\word.pdf", PASSWORD,
                                     L"..\\output_files\\html.html", options, &callback));
    print_result("pdf to rtf",
                 CPDF_StartPDFToRtf(L"..\\input_files\\word.pdf", PASSWORD,
                                    L"..\\output_files\\rtf.rtf", options, &callback));
    print_result("pdf to image",
                 CPDF_StartPDFToImage(L"..\\input_files\\word.pdf", PASSWORD,
                                      L"..\\output_files\\", options, &callback));
    print_result("pdf to txt",
                 CPDF_StartPDFToTxt(L"..\\input_files\\word.pdf", PASSWORD,
                                    L"..\\output_files\\txt.txt", options, &callback));
    print_result("pdf to json",
                 CPDF_StartPDFToJson(L"..\\input_files\\word.pdf", PASSWORD,
                                     L"..\\output_files\\json.json", options, &callback));
    print_result("pdf to markdown",
                 CPDF_StartPDFToMarkdown(L"..\\input_files\\word.pdf", PASSWORD,
                                         L"..\\output_files\\markdown.md", options, &callback));

    options.enable_ocr = true;
    options.transparent_text = true;
    options.languages = languages;
    options.language_count = 1;
    print_result("pdf to searchable pdf",
                 CPDF_StartPDFToSearchablePDF(L"..\\input_files\\word.pdf", PASSWORD,
                                              L"..\\output_files\\pdf.pdf", options, &callback));
    print_result("pdf to ofd",
                 CPDF_StartPDFToOfd(L"..\\input_files\\word.pdf", PASSWORD,
                                    L"..\\output_files\\pdf.ofd", options, &callback));

    run_word_to_pdf(&callback);

    CPDF_GetVersion(version);
    printf("pdfsolid conversion sdk version: %s\n", version);

    CPDF_ReleaseDocumentAIModel();
    CPDF_Release();
    return 0;
}
