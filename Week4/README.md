
> [!WARNING]  
> 今週からディレクトリ構造･宿題の実装方法が少し違います

# ディレクトリ構造

```markdown
.
├── 📂`HW/`
│   ├── 📝 `create_graph.h`
│   ├── 🎮 `create_graph.cpp`: input(wikipedia_dataset)の中身を構造体に入れて渡すプログラム
│   ├── 📝 `HW.h` : HW(1~3)の関数が全て記載されたヘッダファイル
│   ├── 🎮 `HW1.cpp` : BFS 2点間の最短経路長を求めるプログラム
│   ├── 🎮 `HW2.cpp` : ページランクの計算, 上位10位の出力, outputへの書き込み
│   ├── 🎮 `HW3.cpp` : Heuristic 最長経路長を焼きなますプログラム
│   └── 🧠 `main.cpp` : ヘッダファイルから色々includeして動かす
│   └── ✅ `test.cpp` : 関数のテスト用のプログラム
├── 📂 `input/`
│   ├── 📂 `processed_dataset` : HW3用に枝刈りしたデータセットや、ページランク一覧など
│   └── 📂 `wikipedia_dataset` : 元々のデータセット
├── 📂 `output/`
│   └── これから作られる予定です テストの結果などを入れるつもり
├── 🎮 `extra.cpp` : stackを使った再帰DFSの実装
├── 📖 `problem_statement.md` : 問題一覧とざっくりとした方針
└── 📕 `README.md` : **今ここ**

```

# 今週やったこと, 進捗　  
- 　  
　  
# メンターさんに確認していただきたいところ　  
### 宿題
- [ ] [HW1.cpp](https://github.com/ponzudomo/google-step/blob/main/Week4/HW/HW1.cpp) : HW1 (始点と終点を指定された時に最短経路長を返すプログラム)　  
- [ ] [HW2.cpp](https://github.com/ponzudomo/google-step/blob/main/Week4/HW/HW2.cpp) : HW2 (ページランクの計算)　  
- [ ] [HW3.cpp](https://github.com/ponzudomo/google-step/blob/main/Week4/HW/HW3.cpp) : HW3 (最長経路長)　  
### 余裕があれば見ていただきたい部分　  
- [ ] [extra.cpp](https://github.com/ponzudomo/google-step/blob/main/Week4/extra.cpp) : stackを使って、再帰DFSと同じ経路で探索できるDFSを実装　  
　  
# これからやること　  
1. 間違えてクソデカinputをpushしてしまった！！！手遅れっぽくはあるけど対処法を探す　
2. Google Test を使ってみたい  
　  
　  
<span style="font-size:150%">Thank you for your review!</span>

