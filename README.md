# soal-shift-sisop-modul-4-A09-2021

## Daftar Isi

- [Daftar Isi](#daftar-isi)
- [Nomor 1](#nomor-1)
- [Nomor 2](#nomor-2)
- [Nomor 3](#nomor-3)
- [Nomor 4](#nomor-4)

## Nomor 1

[Daftar Isi](#daftar-isi)

### **Deskripsi**

Membuat enkripsi menggunakan Atbash Cipher dengan kriteria:

- Jika sebuah direktori dibuat dengan awalan “AtoZ\_”, maka direktori tersebut akan menjadi direktori ter-encode.
- Jika sebuah direktori di-rename dengan awalan “AtoZ\_”, maka direktori tersebut akan menjadi direktori ter-encode.
- Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.
- Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : **/home/[USER]/Downloads/[Nama Direktori]** → **/home/[USER]/Downloads/AtoZ\_[Nama Direktori]**
- Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

### **Pembahasan**

Pertama membuat fungsi `encrypt1` yang akan dipanggil saat readdir.

Fungsi ini akan mengambil string tanpa ekstensi yang nantinya akan dienkripsi menggunakan Atbash Cipher.
Contoh:

```
halo.txt ⟶ halo
```

Fungsi `encrypt1`:

```c
void encrypt1(char *str)
{
	//encrypt AtoZ
	if (strcmp(str, ".") == 0)
		return;
	if (strcmp(str, "..") == 0)
		return;

	printf("LAGI ENCRYPT %s\n", str);

	int lastIndex = strlen(str);
	for (int i = lastIndex; i >= 0; i--)
	{
		if (str[i] == '/')
			continue;
		if (str[i] == '.')
		{
			lastIndex = i;
			break;
		}
	}
	mirrorUtil(str, lastIndex);
	printf("FINAL %s\n", str);
}
```

Selanjutnya ada fungsi `decrypt1` yang akan dipanggil saat getattr, readdir, unlink, dan rmdir.

Sama seperti fungsi `encrypt1`, fungsi ini hanya akan mengambil string tanpa ekstensi yang akan didekripsi menggunakan Atbash Cipher. Contoh:

```
AtoZ_tes/szol ⟶ /szol
```

Fungsi `decrypt1`:

```c
void decrypt1(char *str)
{
	//decrypt AtoZ_
	if (strcmp(str, ".") == 0)
		return;
	if (strcmp(str, "..") == 0)
		return;

	if (strstr(str, "/") == NULL)
		return;

	char *fileName = strstr(str, "/");

	printf("LAGI DECRYPT %s - %s\n", str, fileName);

	int lastIndex = strlen(fileName);
	for (int i = lastIndex; i >= 0; i--)
	{
		if (fileName[i] == '/')
			break;
		if (fileName[i] == '.')
		{
			lastIndex = i;
			break;
		}
	}

	mirrorUtil(fileName + 1, lastIndex);
}
```

String tanpa ekstensi yang didapat dari kedua fungsi diatas akan digunakan pada fungsi `mirrorUtil`. Karena enkripsinya seperti membalik abjad, fungsi ini dapat digunakan sekaligus untuk enkripsi maupun dekripsi.

Fungsi `mirrorUtil`:

```c
void mirrorUtil(char *str, int safeIndex)
{
	for (int j = 0; j < safeIndex; j++)
	{
		int alphabetCount = 26;
		char awal = str[j];

		if (str[j] >= 65 && str[j] <= 90)
		{
			str[j] = str[j] - 65 + 1;
			str[j] = alphabetCount - str[j];
			str[j] += 65;
		}
		else if (str[j] >= 97 && str[j] <= 122)
		{
			str[j] = str[j] - 97 + 1;
			str[j] = alphabetCount - str[j];
			str[j] += 97;
		}

		printf("%c JADI %c\n", awal, str[j]);
	}
}
```

Terakhir, setiap folder yang dibuat atau direname dengan awalan `AtoZ_` dicatat dalam log menggunakan fungsi `AtoZLog`

Fungsi `AtoZLog`:

```c
void AtoZLog(const char *previousPath, const char *newPath)
{
	FILE *logFile = fopen("/home/rizqitsani/AtoZ.log", "a");
	fprintf(logFile, "%s → %s\n", previousPath, newPath);

	fclose(logFile);
}
```

### **Kendala**

Sempat kebingungan menentukan tempat fungsi decrypt harus dipanggil sehingga folder Fuse menjadi kosong.

### **Screenshot**

## Nomor 2

[Daftar Isi](#daftar-isi)

### **Deskripsi**

- Jika sebuah direktori dibuat dengan awalan “RX\_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).
- Jika sebuah direktori di-rename dengan awalan “RX\_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).
- Apabila direktori yang terencode di-rename (Dihilangkan “RX\_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.
- Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).
- Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal. Sebagai contoh, Suatu_File.txt berukuran 3 kiloBytes pada directory asli akan menjadi 3 file kecil yakni:

  - Suatu_File.txt.0000
  - Suatu_File.txt.0001
  - Suatu_File.txt.0002

  Ketika diakses melalui filesystem hanya akan muncul Suatu_File.txt

### **Pembahasan**

### **Kendala**

### **Screenshot**

## Nomor 3

[Daftar Isi](#daftar-isi)

### **Deskripsi**

- Jika sebuah direktori dibuat dengan awalan “A*is_a*”, maka direktori tersebut akan menjadi sebuah direktori spesial.
- Jika sebuah direktori di-rename dengan memberi awalan “A*is_a*”, maka direktori tersebut akan menjadi sebuah direktori spesial.
- Apabila direktori yang terenkripsi di-rename dengan menghapus “A*is_a*” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.
- Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “AtoZ*” maupun “RX*” namun masing-masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive “AtoZ*” dan “RX*” tetap berjalan pada subdirektori).
- Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi lowercase insensitive dan diberi ekstensi baru berupa nilai desimal dari binner perbedaan namanya.

  Contohnya jika pada direktori asli nama filenya adalah “FiLe_CoNtoH.txt” maka pada fuse akan menjadi “file_contoh.txt.1321”. 1321 berasal dari biner 10100101001.

### **Pembahasan**

### **Kendala**

### **Screenshot**

## Nomor 4

[Daftar Isi](#daftar-isi)

### **Deskripsi**

Membuat log system dengan kriteria:

- Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.
- Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.
- Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.
- Sisanya, akan dicatat pada level INFO.
- Format untuk logging yaitu:

  `[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]`

  Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan

  ```
  INFO::28052021-10:00:00:CREATE::/test.txt
  INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt
  ```

### **Pembahasan**

### **Kendala**

### **Screenshot**
