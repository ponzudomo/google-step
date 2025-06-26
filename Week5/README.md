
> [!WARNING]  
> 今週もまたディレクトリ構造･宿題の実装方法が少し違います

# ディレクトリ構造

<br>

```markdown

.
├── 📂`HW/`
│   ├── 🎮 `01_simulated-annealing.cpp` : まずは貪欲 → 2-optで焼きなまし
│   ├── 🎮 `02_random-init.cpp` : 初期解をランダムに生成
│   ├── 🎮 `03_multi-neighbors.cpp` : 近傍の方法を増やす
│   ├── 📝 `hw.h` : {解法01~03}.cppの関数が全て記載されたヘッダファイル
│   ├── 🤖 `io_example.cpp`: Copilotに入出力のやり方を聞いたら出してくれました 100%Gemini製です
│   ├── 🎮 `io.cpp`: ファイル入出力 +α (構造体の実装など)
│   ├── 📝 `io.h` : io.cppのヘッダファイル
│   ├── 🧠 `main.cpp` : ヘッダファイルから色々includeして動かす
│   └── ✅ `test.cpp` : 関数のテスト用のプログラム (作らないかも)
├── 📂 `materials/` : もらった教材 (下にソースあり / 詳細は `problem_statement.md`)
│   ├── 📃 `input_{1~6}.csv`
│   ├── 📃 `output_{1~6}.csv`
│   └── 🐍 `output_verifier.py`
├── 📖 `problem_statement.md` : 問題文とmaterialsの使い方、ざっくりとした方針
└── 📕 `README.md` : **今ここ**

```

<br>

 `materials/` は [GitHub - hayatoito/google-step-tsp](https://github.com/hayatoito/google-step-tsp/tree/main) より　  
　   
---
　   
# 今週やったこと, 進捗　  
- 　  
　  
## メンターさんに確認していただきたいところ　  
### 宿題
- [ ] [01_simulated_annealing.cpp](https://github.com/ponzudomo/google-step/blob/main/Week5/HW/01_simulated_annealing.cpp) : まずは貪欲で初期解生成 → 2-optで焼きなまし　  
- [ ] [02_random-init.cpp](https://github.com/ponzudomo/google-step/blob/main/Week5/HW/02_random-init.cpp) : 初期解をランダムに生成　  
- [ ] [03_multi-neighbors.cpp](https://github.com/ponzudomo/google-step/blob/main/Week5/HW/03_multi-neighbors.cpp) : 近傍の方法を増やす　  
　  
## これからやること　  
1. 
　  
　  
---
　   
# Bibliography
## Webページ
- [AtCoder - AtCoder Heuristic First-step Vol.1](https://atcoder.jp/contests/ahf1)
- [GitHub - hayatoito/google-step-tsp](https://github.com/hayatoito/google-step-tsp/tree/main)
- [Qiita - 直感でわかる、ヒューリスティック問題の羅針盤 ～貪欲法から山登り法まで～](https://qiita.com/square1001/items/84604f79f55ff10d99b0)
- [Qiita - 巡回セールスマン問題をいろんな近似解法で解く（その1: 総当たり法とグリーディ法）](https://qiita.com/take314/items/dc2e6cf6d97889923c8b)
- [Qiita - 巡回セールスマン問題をいろんな近似解法で解く（その2: 局所探索法と2-opt近傍）](https://qiita.com/take314/items/33843f980c4cbab140ac)
- [Qiita - 巡回セールスマン問題をいろんな近似解法で解く（その3: ランダム化最近近傍法とGRASP法）](https://qiita.com/take314/items/cf12c29aef7bf8e43eeb)
- [Qiita - 巡回セールスマン問題をいろんな近似解法で解く（その4: 反復局所探索法）](https://qiita.com/take314/items/8cdaf32e761665a54799)
- [Qiita - 巡回セールスマン問題をいろんな近似解法で解く（その5: 焼きなまし法）](https://qiita.com/take314/items/7eae18045e989d7eaf52)
- [Qiita - 巡回セールスマン問題をいろんな近似解法で解く（その6: さまざまな近傍）](https://qiita.com/take314/items/69b93481403feb857d6e)
- 
　  
## 本
- [競技プログラミングの鉄則(鉄則本)](https://www.amazon.co.jp/%E7%AB%B6%E6%8A%80%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%81%AE%E9%89%84%E5%89%87-%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0%E5%8A%9B%E3%81%A8%E6%80%9D%E8%80%83%E5%8A%9B%E3%82%92%E9%AB%98%E3%82%81%E3%82%8B77%E3%81%AE%E6%8A%80%E8%A1%93-Compass-Books%E3%82%B7%E3%83%AA%E3%83%BC%E3%82%BA-%E7%B1%B3%E7%94%B0/dp/483997750X)
- [プログラミングコンテストチャレンジブック(蟻本)](https://www.amazon.co.jp/%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%82%B3%E3%83%B3%E3%83%86%E3%82%B9%E3%83%88%E3%83%81%E3%83%A3%E3%83%AC%E3%83%B3%E3%82%B8%E3%83%96%E3%83%83%E3%82%AF-%E7%AC%AC2%E7%89%88-%EF%BD%9E%E5%95%8F%E9%A1%8C%E8%A7%A3%E6%B1%BA%E3%81%AE%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0%E6%B4%BB%E7%94%A8%E5%8A%9B%E3%81%A8%E3%82%B3%E3%83%BC%E3%83%87%E3%82%A3%E3%83%B3%E3%82%B0%E3%83%86%E3%82%AF%E3%83%8B%E3%83%83%E3%82%AF%E3%82%92%E9%8D%9B%E3%81%88%E3%82%8B%EF%BD%9E-%E7%A7%8B%E8%91%89%E6%8B%93%E5%93%89/dp/4839941068)
　  
<span style="font-size:150%">Thank you for your review!</span>

