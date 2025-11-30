# 🟩 **OpenGL + FreeGLUT Setup Guide (Windows & Ubuntu)**

## 📦 Required Packages

| Windows (MSYS2)                   | Ubuntu             |
| --------------------------------- | ------------------ |
| `mingw-w64-ucrt-x86_64-toolchain` | `build-essential`  |
| `mingw-w64-ucrt-x86_64-freeglut`  | `freeglut3-dev`    |
| _(auto-installed)_ OpenGL libs    | `libglu1-mesa-dev` |
| _(auto-installed)_ GLU/GL headers | `mesa-utils`       |

---

## 📌 C++ Compile Template

**Ubuntu:**

```bash
g++ <file>.cpp -lGL -lGLU -lglut -o dest/output
./dest/output
```

**Windows (MSYS2):**

```bash
g++ <file>.cpp -lopengl32 -lfreeglut -o dest/output.exe
./dest/output.exe
```

---

## 🐧 **Ubuntu Setup Guide**

### 1️⃣ Update system

```bash
sudo apt update
```

### 2️⃣ Install dependencies

```bash
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev mesa-utils
```

### 3️⃣ Verify OpenGL

```bash
glxinfo | grep "OpenGL"
```

### 4️⃣ Compile a program

```bash
g++ main.cpp -lGL -lGLU -lglut -o app
./app
```

---

## 🪟 **Windows Setup Guide (MSYS2 Recommended)**

### 1️⃣ Install MSYS2

✅ Using curl (works on all Windows 10/11)

```bash
curl -L -o msys2.exe https://mirror.msys2.org/distrib/x86_64/msys2-x86_64-20240727.exe
```

Or,
Download: [https://www.msys2.org/](https://www.msys2.org/)
Open **MSYS2 UCRT64** terminal.

### 2️⃣ Update packages

```bash
pacman -Syu
# close terminal, reopen
pacman -Su
```

### 3️⃣ Install compiler + FreeGLUT

```bash
pacman -S mingw-w64-ucrt-x86_64-toolchain
pacman -S mingw-w64-ucrt-x86_64-freeglut
```

### 4️⃣ Compile a program

```bash
g++ main.cpp -lopengl32 -lfreeglut -o app.exe
./app.exe
```

---
