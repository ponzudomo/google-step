# Week5 Homework - TSP

## 問題文

> TSPを解いてみよう
> 異なる7つの規模(N=5~2048)のインスタンスが用意されている
> 各インスタンスごとに入力ファイル（input_{0-6}.csv）と出力ファイル（output_{0-6}.csv）がある
> 自作プログラムで各TSPを解き、output_{0-6}.csvを書き換える
> 経路長（path length）をスコアボードに手動で記入する


## 付属ツール（`materials/`）

- Python3のサンプルソルバー（`solver_random.py`, `solver_greedy.py` など）
- 出力検証スクリプト（`output_verifier.py`）
- ビジュアライザWebページ（`visualizer/`）

[GitHub hayatoito/google-step-tsp](https://github.com/hayatoito/google-step-tsp/tree/main) より


### Inputs
CSVファイルとして渡される　  
カンマ区切りでdouble型のx, y座標を受け取る　  
　  
Input Example:　  

```
x,y
214.98279057984195,762.6903632435094
1222.0393903625825,229.56212316547953
792.6961393471055,404.5419583098643
1042.5487563564207,709.8510160219619
150.17533883877582,25.512728869805677
```

### Outputs
既存のCSVファイルに上書きする形で出力する　  
最初の行は `index`, あとは数字　  
　  
Output (Solution) Example:　  

```
index
0
2
3
1
4
```


### `output_verifier.py` の使い方

ターミナルから使う　  

```bash
python Week5/materials/output_verifier.py <input.csvへのパス> <output.csvへのパス>
```

多分標準出力に出てくる　  


## 実装方針

### ポイント

- スタートとゴールが決まっていない！！　  
　→ ~~勝手に始点を決めて輪になるように解を生成して、一番長い辺を切ればいい~~ これじゃ最適解にはならない
　→ 普通に始点終点も焼きなます対象にしてみる
- 
- 


### 作るファイル

- 入出力(`io.cpp`)
- 
- Heuristic
　1. まずは貪欲初期解 → 2-optで焼きなまし(`01_simulated_annealing.cpp`)
　2. 初期解をランダムに生成(`02_random-init.cpp`)
　3. 近傍の方法を増やす(`03_multi-neighbors.cpp`)

