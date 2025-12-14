# Simulasi Kolisi Partikel

Proyek ini adalah simulasi kolisi partikel 2D menggunakan SFML, dengan arsitektur modular untuk mendukung berbagai algoritma deteksi kolisi. Saat ini tersedia algoritma Brute Force, QuadTree (placeholder), dan Spatial Hash. Aplikasi menampilkan partikel bergerak dalam jendela dan mengizinkan penambahan/pengurangan partikel serta pergantian algoritma secara runtime.

## Ringkasan Proyek

- Tujuan: Mensimulasikan interaksi partikel (tabrakan elastis sederhana), menguji dan membandingkan kinerja berbagai algoritma deteksi kolisi.
- Bahasa & Library: C++17, SFML (`sfml-graphics`, `sfml-window`, `sfml-system`).
- Platform: Linux (Makefile disertakan), dapat disesuaikan ke OS lain dengan toolchain yang sesuai.

## Fitur Utama

- Arsitektur modular untuk detektor kolisi melalui antarmuka `ICollisionDetector`.
- Implementasi algoritma:
  - Brute Force: pemeriksaan pasangan O(n²).
  - QuadTree: saat ini placeholder (perlu pengembangan struktur quadtree sesungguhnya).
  - Spatial Hash: grid spasial untuk mengurangi jumlah pasangan yang diperiksa.
- Pergantian algoritma saat runtime tanpa reset partikel.
- Kontrol interaktif: tambah/kurangi partikel, toggle fullscreen, dan ganti algoritma.

## Struktur Folder

```
include/
	CollisionDetector.hpp      // Antarmuka detektor kolisi
	BruteForceDetector.hpp     // Header brute force
	QuadTreeDetector.hpp       // Header quadtree (placeholder)
	SpatialHashDetector.hpp    // Header spatial hash
	Particle.hpp               // Model partikel
	Simulation.hpp             // Orkestrasi simulasi

src/
	BruteForceDetector.cpp
	QuadTreeDetector.cpp
	SpatialHashDetector.cpp
	Particle.cpp
	Simulation.cpp
	main.cpp

assets/
	font/                      // Font SFML yang digunakan

Makefile
bin/
	collision-sim              // Binary release
	collision-sim-debug        // Binary debug (opsional)
```

## Arsitektur & Desain

- `Particle`: menyimpan posisi `(x, y)`, kecepatan `(vx, vy)`, massa, radius (`sqrt(mass)*20`), serta update gerak dan resolusi tabrakan elastis sederhana.
- `ICollisionDetector`: antarmuka dengan metode `detectAndResolve(std::vector<Particle>&)` untuk mendeteksi dan menyelesaikan tabrakan.
- `BruteForceDetector`: implementasi O(n²) memeriksa semua pasangan `(i, j)` dengan `j > i`.
- `QuadTreeDetector`: placeholder; saat ini menggunakan logika brute force. Dapat diganti dengan implementasi quadtree yang membangun partisi ruang dan query tetangga.
- `SpatialHashDetector`: menggunakan grid (hash) dengan ukuran sel konfigurable untuk mengelompokkan partikel; memeriksa kolisi intra-sel dan sel tetangga untuk memangkas jumlah pasangan.
- `Simulation`: mengelola koleksi partikel dan shape SFML, update gerak dan delegasi deteksi kolisi ke detektor terpilih, serta rendering.
- `main.cpp`: UI HUD ringan (teks jumlah partikel dan algoritma), event loop SFML, kontrol keyboard, dan life-cycle window.

## Kontrol Aplikasi

- `B`: Ganti algoritma ke Brute Force.
- `Q`: Ganti algoritma ke QuadTree (placeholder).
- `H`: Ganti algoritma ke Spatial Hash.
- `+` (Equal/Add): Tambah 500 partikel.
- `-` (Hyphen/Subtract): Hapus 500 partikel tertua.
- `F`: Toggle Fullscreen/Windowed.
- `Esc`: Keluar.

Catatan: Pergantian algoritma berusaha untuk tidak mereset partikel. Semua perubahan terjadi pada set partikel yang sama.

## Build & Menjalankan

Pastikan SFML terpasang (dev libraries). Contoh di distro berbasis Debian/Ubuntu:

```bash
sudo apt-get install libsfml-dev
```

Build dan jalankan:

```bash
make -C "/mnt/c/Users/mahdi/Desktop/Dasprog/Collision SImulation"
./bin/collision-sim
```

Target Makefile:

- `make` atau `make release`: membangun binary rilis `bin/collision-sim`.
- `make debug`: membangun binary debug `bin/collision-sim-debug`.

## Performa & Perbandingan Algoritma

- **Brute Force**: Kompleksitas O(n²). Untuk jumlah partikel besar, akan menurun drastis.
- **Spatial Hash**: Secara praktis memangkas pasangan yang harus diperiksa dengan hanya mengecek intra-sel dan sel tetangga. Cocok untuk distribusi partikel yang relatif merata.
- **QuadTree (placeholder)**: Saat ini belum menerapkan struktur quadtree sesungguhnya; akan menunjukkan kinerja mirip brute force sampai diimplementasikan.

Pendekatan Evaluasi:

- Amati FPS saat menambah partikel (`+`) dan saat berpindah algoritma (`B`, `Q`, `H`).
- Spatial Hash biasanya lebih stabil saat jumlah partikel meningkat karena pengurangan kandidat pasangan.
- Untuk evaluasi yang lebih robust, dapat ditambahkan mode benchmarking yang mengukur: rata-rata FPS, 1% low FPS (mengindikasikan stutter), dan waktu `update` per frame (ms). Saat ini HUD default hanya menampilkan FPS dan informasi algoritma.

Tips Tuning Spatial Hash:

- `cellSize` sebaiknya mendekati diameter rata-rata partikel (≈ `2 * sqrt(mass) * 20`). Nilai default dapat diubah di `SpatialHashDetector`.

## Catatan Platform

- Jika terjadi error GLX/MESA (driver grafis), itu terkait lingkungan eksekusi, bukan logika simulasi. Jalankan pada mesin dengan driver OpenGL/SFML yang baik untuk hasil optimal.

## Rencana Pengembangan Lanjutan

- Implementasi quadtree sesungguhnya (inserter dinamis, query tetangga, rebuild per frame atau update incremental).
- Mode benchmarking otomatis: siklus algoritma dan log metrik (avg FPS, 1% low, p95/p99) ke file CSV.
- Pengaturan HUD opsional untuk menampilkan metrik tambahan (collision ms, update ms) dan indikator algoritma yang aktif.

## Lisensi

Proyek ini untuk keperluan pembelajaran. Silakan sesuaikan lisensi sesuai kebutuhan Anda.
