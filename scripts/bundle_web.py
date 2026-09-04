#!/usr/bin/env python3
"""
bundle_web.py: Inlines and bundles HTML, CSS, and JS into include/ui/WebAssets.h
"""

import os
import gzip

WEB_DIR = os.path.join(os.path.dirname(__file__), "..", "web")
OUTPUT_HEADER = os.path.join(os.path.dirname(__file__), "..", "include", "ui", "WebAssets.h")

def bundle():
    index_html_path = os.path.join(WEB_DIR, "index.html")
    style_css_path = os.path.join(WEB_DIR, "style.css")
    app_js_path = os.path.join(WEB_DIR, "app.js")

    with open(index_html_path, "r", encoding="utf-8") as f:
        html = f.read()
    with open(style_css_path, "r", encoding="utf-8") as f:
        css = f.read()
    with open(app_js_path, "r", encoding="utf-8") as f:
        js = f.read()

    # Inline CSS and JS into a single standalone HTML document
    inlined_html = html.replace('<link rel="stylesheet" href="style.css">', f'<style>\n{css}\n</style>')
    inlined_html = inlined_html.replace('<script src="app.js"></script>', f'<script>\n{js}\n</script>')

    # Generate C++ Header
    header_content = f"""/**
 * @file WebAssets.h
 * @brief Auto-generated standalone Web UI bundle for ESP32-RF-SWORD
 * @note Generated automatically by scripts/bundle_web.py
 */

#pragma once

#include <stddef.h>

namespace WebAssets {{

const char INDEX_HTML[] = R"rawliteral(
{inlined_html}
)rawliteral";

inline const char* getIndexHtml() {{
    return INDEX_HTML;
}}

}} // namespace WebAssets
"""

    os.makedirs(os.path.dirname(OUTPUT_HEADER), exist_ok=True)
    with open(OUTPUT_HEADER, "w", encoding="utf-8") as f:
        f.write(header_content)

    print(f"[+] Successfully generated {OUTPUT_HEADER} ({len(inlined_html)} bytes)")

if __name__ == "__main__":
    bundle()
