# shuupure
就職活動用作品です。
以下の機能を実装しています。
- 被写界深度
- モーションブラー
- カスケードシャドウ
- 法線マップによる水面の表現
- 画面をpng画像として保存する機能
- インスタンシングによる3Dモデルの描画
- 視錐台カリング
## 操作説明
- 1, 2キー
  - ボケ調整
- 3, 4キー
  - 焦点距離調整
- 5, 6キー
  - ピントがあう範囲調整
- I, J, K, Lキー
  - カメラ回転
- Tキー
  - 画面を画像で保存
    - photoフォルダが作成され、そこに保存されます
- スペースキー
  - 水面の法線マップのオンオフ切り替え
    - オフにすることで環境マッピングが分かりやすくなります


<img width = "400" alt = "スクリーンショット 2025-01-30 181152" src = https://github.com/user-attachments/assets/f6be31a4-3967-4533-8ce1-01bdd7f676b6>
<img width = "400" aly = "スクリーンショット 2025-01-30 181210" src = https://github.com/user-attachments/assets/f9bc1e88-a89e-43c1-880f-f5f15eff16fe>

画面右上にある四角はチェックボックスになっており、クリックすると被写界深度のテクスチャとシャドウマップに使っているテクスチャを表示することができます。
