# 🔍 Mini Search Engine with Autocomplete and Word Break (C++)

## 📌 Overview

This project implements a **mini search engine** in C++ with the following features:

* **Text Preprocessing**: Converts text to lowercase, removes punctuation, handles plurals, and removes stop words.
* **Trie Data Structure**: Stores words for **autocomplete** and **word segmentation**.
* **Inverted Index**: Maps words to web pages for efficient search.
* **Search Engine**:

  * Ranks results by exact matches, frequency, and token matches.
  * Supports query tokenization and word breaking.
* **Autocomplete System**: Provides real-time word suggestions while typing.
* **Console UI**: Arrow keys for cycling suggestions and **Tab** for autocomplete.

---

## ⚙️ Features

1. **Text Processing**

   * Converts input to lowercase.
   * Removes punctuation.
   * Eliminates common stopwords (`the`, `is`, `and`, etc.).
   * Handles singular/plural forms (`cars` → `car`).

2. **Trie Implementation**

   * Insert and store tokens.
   * Autocomplete suggestions based on prefix.
   * Word segmentation using recursive word break.

3. **Inverted Index**

   * Maps tokens to web pages.
   * Allows efficient word-based searching.

4. **Search Engine Ranking**

   * Prioritizes **exact matches**.
   * Considers frequency and token match count.
   * Displays ranked results.

5. **Interactive Console**

   * Live typing with autocomplete.
   * Arrow keys (`↑` / `↓`) to navigate suggestions.
   * Tab key (`↹`) to accept a suggestion.

---

## 🖥️ How to Run

### Requirements

* **Windows OS** (uses `windows.h` and `conio.h`).
* C++17 or later.

### Compilation

```bash
g++ search_engine.cpp -o search_engine
```

### Execution

```bash
./search_engine
```

---

## 📊 Example Usage

```
Type a sentence: search eng
Suggestion: search engine
```

🔹 Press **Tab** → completes to `search engine`
🔹 Press **Enter** → search engine runs query and returns ranked web pages

---

## 📚 Project Structure

* **Text Preprocessing**: `toLower()`, `removePunctuation()`, `toSingular()`
* **Trie**: Stores words and provides autocomplete + search.
* **WebPage Class**: Holds webpage content and word frequencies.
* **InvertedIndex Class**: Efficient token → page mapping.
* **SearchEngine Class**: Manages search, ranking, and display.
* **UI Functions**: Cursor positioning, text coloring, interactive input.

---

## 🚀 Future Improvements

* Add **phrase-based search**.
* Implement **TF-IDF ranking** instead of raw frequency.
* Support **Linux/Mac** by replacing `windows.h` and `conio.h`.
* Extend autocomplete with **fuzzy matching**.
