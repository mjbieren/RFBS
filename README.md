# 🧹 RFBS (Remove Fasta Block Sequences)

**RFBS** is a lightweight tool designed to clean FASTA files by removing specified sequences based on strain names or specific headers. It works by reading a list of target entries (strains or exact headers) from a user-defined file and excluding those sequences from the output.

![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)  
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey)

---

## 📋 Overview

The tool identifies complete FASTA blocks (i.e. header + sequence) to **remove** based on a header list provided by the user.

The removal criteria are:
- **Strain-level removal**: Remove all entries from a given species/strain.
- **Header-level removal**: Remove only specific headers (including gene identifiers).

The output is a cleaned set of FASTA files written to a user-defined output directory.

---

## 🧾 Input Header File Format

The input header file is a plain text file where each line defines either:
- A **strain name** to remove all sequences for that strain.
- A **specific header** (strain + gene ID) to remove one sequence.

Each line must follow this format:
```
StrainOrHeaderName<TAB>Symbol
```

Where:
- `+` → Remove all headers that match the strain name  
- `-` → Remove only the specific header (strain + gene ID)

### 🔍 Example
Aagrestis_BONN +
Aagrestis_OXF@17242 -


- `Aagrestis_BONN	+` → Removes **all headers** containing `Aagrestis_BONN`
- `Aagrestis_OXF@17242	-` → Removes only the **single header** `Aagrestis_OXF@17242`

---

## ⚙️ Command Line Options

Run the tool using the following structure:
```
RFBS.out -f <FastaFileFolder> -h <HeaderFilePath> -r <OutputFolderPath> [-p]
```


### Arguments

| Flag | Description |
|------|-------------|
| `-f` | **Input folder** with `.fa` or `.fasta` files. **(Required)** |
| `-h` | **Header file** that contains the list of headers/strains to remove. **(Required)** |
| `-r` | **Output folder** where the filtered FASTA files will be saved. **(Required)** |
| `-p` | Optional flag. If set, FASTA headers will be **converted to PhyloPyPruner format**. |

---

## ⚙️ Internal Workflow

1. **Parse command-line arguments**
2. **Read FASTA files and header removal list**
3. **Match and flag FASTA blocks for removal**:
   - Entire strains
   - Specific headers
4. **Write filtered FASTA files to output folder**
5. **Clean up memory**
6. **Done**

---

## 📂 Output

Filtered FASTA files will be written to the provided output folder, with only the unflagged headers retained.

---

## 🛠️ Compilation Notes

Precompiled static executables available:  

- Debian 12: [RFBS_Static_Debian.out](https://github.com/mjbieren/RFBS/blob/main/Sources/Executables/RFBS_Static_Debian.out)  
- Rocky Linux 8: [RFBS_Static_HPC.out](https://github.com/mjbieren/RFBS/blob/main/Sources/Executables/RFBS_Static_HPC.out)  

Or compile from source. (Boost library 1.88, Visual Studio 2019, GCC compiler)

---

## 📧 Contact

**Maaike Jacobine Bierenbroodspot**  
📧 maaikejacobine.bierenbroodspot@uni-goettingen.de

---




