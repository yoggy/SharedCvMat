SharedCvMat
===========

共有メモリを使ってcv::Matを同一マシン上で共有するライブラリ。Windows用。

1台のカメラ映像を複数のプログラムから共有したいときなどにどうぞ。

HowToUse
===========
詳細はSampleCapture_main.cpp, SampleViewer_main.cppを参照。

<pre>
  // 初期化。共有名、画像サイズ、チャンネル数を指定
  SharedCvMat shared_img("capture_image", 640, 480, 3, false);
  
  cv::Mat image;
  
  // イメージを共有メモリへアップロード
  shared_img.upload(image);
  
  // 共有メモリからイメージをダウンロード
  shared_img.download(image);
</pre>
