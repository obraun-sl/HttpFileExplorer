# Http File Explorer with Tree View, Breadcrumbs & Search

A lightweight, cross-platform HTTP file explorer written in **C++ using Qt5** and **cpp-httplib**.

Provides an interactive UI to browse, search, and download files via any web browser.

---

## 🚀 Features

✅ Directory **tree view** with collapsible folders  
✅ Clean **breadcrumb navigation**  
✅ 🔍 **Search/filter** files and folders by name  
✅ 📄 File download via HTTP  
✅ 📁 Folder and file icons (emoji-based)  
✅ 🖥️ No browser-side dependencies (vanilla JS/CSS)  
✅ Cross-platform (Linux, macOS, Windows)  

---

## 🛠 Build Instructions

### 📦 Requirements

- Qt5 (`libqt5core5a`, `qtbase5-dev`, etc.)
- CMake (≥ 3.10)
- A C++14-compatible compiler
- [cpp-httplib](https://github.com/yhirose/cpp-httplib) (included as header-only)

---

### Build instructions

```bash
# Create build directory
mkdir build && cd build

# Run CMake
cmake ..

# Build it
make
```

---

## Running the Server

```bash
./HttpFileExplorer --directory=/path/to/your/folder --port=8080
```

Then open a browser:

```
http://localhost:8080
```

You can also access it remotely via the machine’s IP.

---

## 📂 Example Usage

- Navigate folders by clicking on 📁 folder names.
- Click 📄 file names to download the file on your PC
- Use the search bar to filter files and folders.
- Breadcrumb links help you jump back up in the folder hierarchy.

---

## 📃 Example Command

```bash
./HttpFileExplorer -d ~/Downloads -p 8080
```

---

## 📌 Notes

- This project uses `cpp-httplib.h`, included in the source. You can update it from [cpp-httplib GitHub](https://github.com/yhirose/cpp-httplib).
- Tested with Qt 5.15+ on Linux & Jetson.

---

## 🧪 Ideas for Future Enhancements

- 🔐 Authentication (username/password)
- 📦 Download multiple / selected files as `.zip`
- 📤 Upload file support (via drag & drop)

---

## 📜 License

MIT License — use freely for personal and commercial projects.

---

## 🙏 Acknowledgments

- kudos to [cpp-httplib](https://github.com/yhirose/cpp-httplib) for the amazing single-header HTTP server.

---
 
