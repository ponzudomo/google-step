# Week7 - ディレクトリ構造

<br>

```markdown

.
├── 📂 `hikalium-san/` : いただいた教材 (下にソースあり / 詳細は `problem_statement.md`)
│   └── 📕 `README.md` : ここに全てが書いてある
├── 📂`HW/`
│   ├── 🎮 `best-fit-malloc.c` : Best Fitのmalloc
│   ├── 🎮 `common.c` : 共通部分は邪魔なので取り出し
│   ├── 🎮 `first-fit-malloc.c` : First Fitのmalloc
│   ├── 🎮 `free-list-bin.c` : Free List Bin
│   ├── 🎮 `merge-free-list.c` : Free List追加時にsort&merge
│   ├── 🎮 `worst-fit-malloc.c` : Worst Fitのmalloc
│   └── ...
├── 📖 `problem_statement.md` : 問題概要とざっくりとした方針
└── 📕 `README.md` : **今ここ**

```

- 見たいものをピンポイントで探せるように、機能ごとにファイルを分けています
- `malloc.c` だと一つのファイルにベタ書きしなきゃいけないっぽいので、随所から使いたいものを拾って貼り付けています(こっちは常にぐちゃぐちゃなので見る必要はないです HWファイルに自分の実装は全てまとめておきます)
- 構造がアレなのは、余裕があればどうにかする予定です...。


---

# 今週やったこと, 進捗　  
- first fit, worst fitのmallocを実装した　  
- free list binみたいなのを実装した はずが `[run] Killed: 9` (Out of Memory) に (たぶんbinの分け方の問題)　  
- あわてて `merge-free-list.c` を書いた　  
- 永遠にrunが終わらないのでひとまず提出します すみません　  
- binへの空き地挿入時とbinからの借入時にサイズ判定をしていなかったので追加 まだrunは終わらない　  
- free list binの管理にて、要求サイズに理想的なbinしか渡していなかったので、適切な空き地が見つからなかった場合により大きなbinを探しに行けるように修正 runは終わるも `Abort trap: 6` (An allocated object is broken)　  
　  
# メンターさんに確認していただきたいところ　  
### 宿題
- [ ] [best-fit-malloc.c](https://github.com/ponzudomo/google-step/blob/main/Week7/HW/best-fit-malloc.c)　  
- [ ] [worst-fit-malloc.c](https://github.com/ponzudomo/google-step/blob/main/Week7/HW/worst-fit-malloc.c)　  
- [ ] [free-list-bin.c](https://github.com/ponzudomo/google-step/blob/main/Week7/HW/free-list-bin.c)　  
### 余裕があれば見ていただきたい部分　  
- [ ] [merge-free-list.c](https://github.com/ponzudomo/google-step/blob/main/Week7/HW/merge-free-list.c)　  
　  
# これからやること　  
1. Bin内でのFree Listの探し方にぶたんにしてみたい　  
2. メタデータに使用状況を付け足しちゃダメなのかな　  
3. いっそガラッと趣向を変えて累積和でメモリ管理するのはどうだろう　  
　  
　  
<span style="font-size:150%">Thank you for your review!</span>

