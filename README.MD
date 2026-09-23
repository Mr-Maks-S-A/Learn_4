# 💻 TT
</div>

> **Примечание:**

### 🖥️ Окружение 
<div align="center">

| Property | Value |
| :--- | :--- |
| **OS** | CachyOS x86_64 (Linux 6.19.9) |
| **DE/WM** | KDE Plasma 6.6.3 / KWin (Wayland) |
| **Shell** | fish 4.5.0 |
| **Terminal** | alacritty 0.16.1 |
| **CPU** | AMD Ryzen 7 3700X @ 4.43 GHz |

</div>


####  Команды для сборки
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug 
```

```bash
cmake --build build --parallel $(nproc)
```

```bash
./build/bin/
```




