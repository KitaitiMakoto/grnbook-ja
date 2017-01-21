= Mroongaの環境の準備

本書では、MySQL及びその他の環境の準備にDockerを使います。OS X El Capitanで確認していますが、異なるOSでも違いはないと思います。

以後、PHPとMySQL、Mroongaを使ってPDF文書の全文検索システムを作成していきますが、ラップトップなどの普段使っているシステムにインストール済みの物を使うと、バージョンが異なって本書の内容が当てはまらなかったり、また、将来別件で異なるバージョンをインストールしたい場合などに手間が増えてしまいます。設定ファイルの競合も問題になるかも知れません。Dockerを使うと、システムの方に影響を与えずに、本書で必要なバージョンのソフトウェアを揃えることができます。不要になった場合には削除することも簡単で、その場合もシステムの方には影響を及ぼしません。

== Dockerのインストール

まず、Dockerその物をインストールしましょう。DockerはOS X、Windows、Linuxそれぞれのプラットフォームで動作します。本番環境として動かすにはLinuxを使うべきですが、本書のようなお試し環境の共有目的ではどの環境で使っても問題ありません。ここでは、本書執筆の際にも使用した、OS Xでのインストール方法を解説します。Linux、Windowsでのインストールについては、それぞれDocker公式サイトでの解説を参照してください。

 * Linuxの場合 ... @<href>{https://docs.docker.com/engine/installation/linux/, Install Docker on Linux distributions}
 * Windowsの場合 ... @<href>{https://docs.docker.com/docker-for-windows/, Get started with Docker for Windows}

=== OS Xへのインストール

Docker公式サイトでは@<href>{https://docs.docker.com/docker-for-mac/, Get started with Docker for Mac}でインストール方法が解説されていますので、以下の方法でうまくいかない場合などには参照してください。

OS XへDockerをインストールするには、公式サイトで配布されているDocker for Macをインストールします。これはDocker関連ツールをまとめたパッケージです。以下の配布ページから「Get Docker for Mac (stable)」と書かれたリンクからダウンロードし、インストーラーでインストールしてください。尚、Docker ToolboxのインストールにはOS X 10.11以上が必要です（2017年1月現在）。

@<href>{https://docs.docker.com/docker-for-mac/#/download-docker-for-mac}

インストールが終わったら、Dockerを起動します。アプリケーションディレクトリーでDockerをダブルタップするか、SpotlightでDockerを検索して実行してください@<fn>{docker-privileges}。するとメニューバーにDockerのクジラのアイコンが表示されます。これによりDockerの起動が確認できました。

//footnote[docker-privileges][初回実行時には「Docker needs privileged access.」というウィンドウに続き、OSのパスワード入力が求められます。これはDockerが管理者権限を要する操作を行うためです。]

== Dockerコンテナの起動

Dockerを使用するには、必要なソフトウェアをインストールし、起動時の動作を定義した「イメージ」が必要です。イメージを取得した後に、そのイメージを雛形としてアプリケーションの動作環境となるコンテナを作ります。一つのイメージからは複数のコンテナを作成して使用することができます。

本書用に必要なソフトウェアをインストールしたDockerイメージを作成してあります。これを元に皆さんのコンテナを作成し、その中でPHPを実行して全文検索システムを作っていきます。

まずはイメージを取得します。ターミナルで以下のコマンドを実行してください。

//list[docker pull][Dockerイメージの取得]{
% docker pull kitaitimakoto/grnbook-mroonga
Using default tag: latest
latest: Pulling from kitaitimakoto/grnbook-mroonga
fdd5d7827f33: Pull complete
a3ed95caeb02: Pull complete
0f35d0fe50cc: Pull complete
627b6479c8f7: Pull complete
67c44324f4e3: Pull complete
1429c50af3b7: Pull complete
8207a1b09d34: Pull complete
f4fd1f72cd2a: Pull complete
8c4074b3c552: Pull complete
d0e29d6de6ea: Pull complete
07672a754971: Pull complete
b0c92bbfd7a4: Pull complete
a42300873aad: Pull complete
2682e716a4cc: Pull complete
dd4fb0863e03: Pull complete
1cf726591e93: Pull complete
9435561d81cb: Pull complete
21da239ddc32: Pull complete
a563b423501c: Pull complete
Digest: sha256:b0c1a8cdde715ff679054eb8d22bf984fad29c285404d8365e135adf73d0bbd3
Status: Downloaded newer image for kitaitimakoto/grnbook-mroonga:latest
//}

これにより、Docker HubというDockerイメージ登録サービスから、本書用のイメージをダウンロードします。1GiB以上あるので時間が掛かります。場合によってはネットワークを共有している同僚などに迷惑を掛けるかも知れませんので、周りの状況をよく見て実施しましょう。

次に、このイメージを元に、あなたのアプリケーションを作成し実施するための仮想環境「コンテナ」を作成します。本書サンプル用の適当なディレクトリーに移動し、以下のコマンドを実行します。

//list[docker create][Dockerコンテナの作成]{
% cd path/to/project
% docker create \
    --name=pdfsearch \
    --publish=8080:80 \
    --volume=$PWD:/var/lib/pdfsearch \
    kitaitimakoto/grnbook-mroonga
69277eea1becda07e4d6314485b6bd48fcfee065b23fa3692ad015d43d5f5c6f
//}

コマンドを実行した結果表示される「@<tt>{69277eea1becda07e4d6314485b6bd48fcfee065b23fa3692ad015d43d5f5c6f}」という行は、このコンテナのIDです。システム全体の中でそれぞれのコンテナを一意に特定するための物で、同じイメージから作成しても、別のコンテナには別のIDが割り振られます。今後、コンテナを操作する時にはこのIDを使用することができます。しかし、これを覚えるのは不可能ですし、一々確認のためのコマンドを実行するのも手間ですので、@<code>{name}オプションにより名前を付けました。以後はIDの代わりに、この名前（@<code>{pdfsearch}）を使用していきます。なお、この名前もシステム内で一意である必要があり、後から同じ名前のコンテナを作ろうとしても失敗します。

@<code>{publish}オプションではポート番号のマッピングを定義しています。コロンの左側にホスト側（Dockerを実行している皆さんのコンピューター）のポート番号を書き、右側にコンテナ内のポート番号を書きます。コンテナ内ではApache HTTP Serverが80番ポートをリッスンしますので、この場合はホスト側の8080番ポートにアクセスするとコンテナ内のApacheに接続できるようになっています。ホスト側のポート番号は他で使用している可能性もありますが、その場合には、次のように@<code>{publish}オプションで変更してください。

//list[change port][8000番ポートを公開する例]{
% docker create \
    --name=pdfsearch \
    --publish=8000:80 \
    --volume=$PWD:/var/lib/pdfsearch \
    kitaitimakoto/grnbook-mroonga
//}

また@<code>{volume}オプションによって、ホスト側とコンテナ内との共有ディレクトリーを指定しています。ここでもコロンの左側（@<code>{$PWD}）がホスト側のディレクトリー、右側（@<code>{/var/lib/pdfsearch}）がコンテナ内のディレクトリーです。ここで指定したディレクトリー以外は、原則としてホスト側とは共有されません@<fn>{share-volume}。右側は、このディレクトリーがApacheのドキュメントルートとなるよう設定しているので、変更しないでください。

最後に、このコンテナを起動しましょう。@<code>{docker start}コマンドを使用します。

//list[docker start][Dockerコンテナの起動]{
% docker start pdfsearch
pdfsearch
//}

実はここまでの@<code>{docker pull}、@<code>{create}、@<code>{start}をまとめて行う、@<code>{docker run}というコマンドがあります。

//list[docker run][Dockerイメージの取得とコンテナの起動]{
% docker run \
    --detach \
    --name=pdfsearch \
    --publish=8080:80 \
    --volume=$PWD:/var/lib/pdfsearch \
    kitaitimakoto/grnbook-mroonga
//}

このように@<code>{docker run}コマンドを実行することで、以下の三つのことが行われます。

 1. Dockerイメージの取得（もしシステム上に存在しない場合）（@<code>{docker pull}）
 2. 取得したDockerイメージを元にしたDockerコンテナの作成（@<code>{docker create}）
 3. 作成したDockerコンテナの起動（@<code>{docker start}）

先に説明した通り、@<code>{name}オプションを指定している場合は、複数回実行すると名前の重複によりコンテナの作成に失敗します。もし指定しなかった場合は、実行の度に新しいコンテナが作成・起動されます。コンテナを起動する二度目以降は@<code>{docker start}を使ってください。

行っていることの説明のため三つのステップで説明しましたが、実際には@<code>{docker run}コマンドを実行するのが簡単でしょう。

起動したコンテナを停止するには@<code>{docker stop}コマンドを実行します。

//list[docker stop][Dockerコンテナの停止]{
% docker stop pdfsearch
pdfsearch
//}

== 動作確認

Dockerコンテナが実際に動作していることを確認しましょう。Dockerコンテナを停止している場合には、@<code>{docker start pdfsearch}で再度起動してください。

このイメージにはPHPが含まれているので、@<code>{docker run}または@<code>{create}を実行したディレクトリーで以下のようなファイルを作成することで動作確認ができます。

//list[info.php][info.php][php]{
<?php
phpinfo();

//}

ブラウザーで @<tt>{http://localhost:8080/info.php} にアクセスしてみましょう。

//image[phpinfo][phpinfo()の実行結果]{
//}

PHPの情報が表示されれば、環境の準備は成功しています。

「Not Found」の画面が表示されてしまう場合は、どこかで間違えてしまったようです。これまでの手順を見ながらやり直してみてください。

@<code>{docker run}をやり直すには、以下のコマンドを実行して一旦Dockerコンテナを停止し、削除する必要があります。

//emlist[Dockerコンテナの停止と削除]{
% docker stop pdfsearch
% docker rm pdfsearch
//}

ありがちな間違いとして、Dockerコンテナとの共有ディレクトリーの指定ミスがあります。@<code>{docker run}（@<code>{docker create}）コマンドの@<code>{volume}オプションをもう一度確認しましょう。@<code>{$PWD}は（@<code>{docker run}を実行した）現在のディレクトリーを意味しますので、そこと別のディレクトリーにファイルを置いた場合は、コンテナ内のPHPが認識できません。事前にプロジェクトのディレクトリーに移動してから実行するようにしましょう。

//footnote[share-volume][原則を外れることもできますが、本書の範囲外のため解説しません。気になる方はDockerの公式サイトなどで@<code>{volume}オプション及び「Dockerfileの@<code>{VOLUME}命令」を調べてみてください。]
