Groongaではじめる全文検索
=========================

大事にしたいこと
----------------

* 初心者向け
* リリース優先

ディレクトリーツリー
--------------------

* src ... ソースファイル。主にここのファイルを編集する。
* images ... 画像ファイル。
* chXX ... アセットファイル。ソースに埋め込むHTMLファイルなど。
* ./*.re ... srcディレクトリー内のファイルを`review-preproc`した物。
* web ... srcディレクトリーからHTMLに変換した物。

HTMLファイルのビルド（執筆中）
------------------------------

ビルドは主にRakeで行っているので

    % rake --tasks

を実行することでやれることを把握できる。

### 全ての章のHTMLファイル

    % rake html

webディレクトリーにHTMLファイルが出来る。

### 各章のHTMLファイル

    % rake chXX

webディレクトリーに当該章のHTMLファイルが出来る。

### 自動ビルド

srcディレクトリーやchXXディレクトリーに変更があると自動でHTMLファイルをビルドするよう、Guardfileを設定している。

    % guard start

とすることでGuardがファイルの監視を始める。

本のビルド
----------

EPUB:

    % review-epubmaker config.yml

PDF:

    % review-pdfmaker config.yml
