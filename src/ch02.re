= Mroongaの環境の準備

本書では、MySQL及びその他の環境の準備にDockerを使います。

以後、PHPとMySQL、Mroongaを使ってPDF文書の全文検索システムを作成していきますが、ラップトップなどの普段使っているシステムにインストール済みの物を使うと、バージョンが異なって本書の内容が当てはまらなかったり、また、将来別件で異なるバージョンをインストールしたい場合などに手間が増えてしまいます。設定ファイルの競合も問題になるかも知れません。Dockerを使うと、システムの方に影響を与えずに、本書で必要なバージョンのソフトウェアを揃えることができます。不要になった場合には削除することも簡単で、その場合もシステムの方には影響を及ぼしません。

== Dockerのインストール

まず、Dockerその物をインストールしましょう。DockerはOS X、Windows、Linuxそれぞれのプラットフォームで動作します。本番環境として動かすにはLinuxを使うべきですが、本書のようなお試し環境の共有目的ではどの環境で使っても問題ありません。

=== OS Xへのインストール

OS XへDockerをインストールするには、公式サイトで配布されているDocker Toolbeltをインストールします。これはDocker関連ツールをまとめたパッケージです。以下の配布ページからダウンロードし、インストーラーでインストールしてください。尚、Docker ToolbeltのインストールにはOS X 10.8以上が必要です（2016年6月現在）。

@<href>{https://www.docker.com/products/docker-toolbox}

インストール中に、インストールするコンポーネントの選択画面があります。本書の内容を試すには「Docker Client」「Docker Machine」「Docker Quickstart Termial」が必要です。また、まだインストールしていない場合は「Oracle VM VirtualBox」も必要になるので、これもインストールしてください。

その他の選択場面については、どれを選んでも本書の内容には影響がありません。

インストールが終わったら、これから使うことになる、Dockerを使うためのターミナルを起動しましょう。LaunchpadやSpotlightでDocker Quickstart Terminalを探して実行してください。初回は様々な環境のセットアップが実行されるため、使用できるようになるまで時間が掛かります。少し待ちましょう。二回目以降はもう少し早く起動できるようになります。以後、「（ホストの）ターミナル」という言葉を使う場合には、こうしてDockerを使う環境を準備したターミナルを指すことにします。

=== Windowsへのインストール

（後で書く）

=== Linuxへのインストール

（後で書く）

== Dockerコンテナの起動

Dockerを使用するには、必要なソフトウェアをインストールし、起動時の動作を定義した「イメージ」が必要です。イメージを取得した後に、そのイメージを雛形としてアプリケーションの動作環境となるコンテナを作ります。一つのイメージからは複数のコンテナを作成して使用することができます。

本書用に必要なソフトウェアをインストールしたDockerイメージを作成してあります。これを元に皆さんのコンテナを作成し、その中でPHPを実行して全文検索システムを作っていきます。

まずはイメージを取得します。ターミナルで以下のコマンドを実行してください。

//list[docker pull][Dockerイメージの取得]{
% docker pull kitaitimakoto/grnbook-mroonga
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

コマンドを実行した結果表示されるのは、このコンテナのIDです。システム全体の中でそれぞれのコンテナを一意に特定するための物で、同じイメージから作成しても、別のコンテナには別のIDが割り振られます。今後、コンテナを操作する時にはこのIDを使用することができます。しかし、これを覚えるのは不可能ですし、一々確認のためのコマンドを実行するのも手間ですので、@<code>{name}オプションにより名前を付けました。以後はIDの代わりに、この名前（@<code>{pdfsearch}）を使用していきます。なお、この名前もシステム内で一意である必要があり、後から同じ名前のコンテナを作ろうとしても失敗します。

@<code>{publish}オプションではポート番号のマッピングを定義しています。コロンの左側にホスト側（Dockerを実行している皆さんのコンピューター、またはOS XやWindowsなどの場合は、Docker Machineで作成した仮想マシン）のポート番号を書き、右側にコンテナ内のポート番号を書きます。コンテナ内ではApacheが80番ポートをリッスンしますので、この場合はホスト側の8080番ポートにアクセスするとコンテナ内のApacheに接続できるようになっています。ホスト側のポート番号は他で使用している可能性もありますが、その場合には変更してください。

また@<code>{volume}オプションによって、ホスト側とコンテナ内との共有ディレクトリーを指定しています。ここでもコロンの左側（@<code>{$PWD}）がホスト側のディレクトリー、右側（@<code>{/var/lib/pdfsearch}）がコンテナ内のディレクトリーです。ここで指定したディレクトリー以外は、原則としてホスト側とは共有されません@<fn>{share-volume}。右側は、このディレクトリーがApacheのドキュメントルートとなるよう設定しているので、変更しないでください。

最後に、このコンテナを起動しましょう。@<code>{docker start}コマンドを使用します。

//list[docker start][Dockerコンテナの起動]{
% docker start pdfsearch
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
//}

== 動作確認

Dockerコンテナが実際に動作していることを確認しましょう。このイメージにはPHPが含まれているので、@<code>{docker run}または@<code>{create}を実行したディレクトリーで以下のようなファイルを作成することで動作確認ができます。

//list[info.php][info.php][php]{
<?php
phpinfo();

//}

ブラウザーでアクセスしてみましょう。OS X、Windowsの場合は、アクセス先のIPアドレスは、@<code>{docker-machine env default}コマンドで確認することができます。

//emlist[Dockerが動作している仮想マシンのIPアドレス確認]{
% docker-machine env default
export DOCKER_TLS_VERIFY="1"
export DOCKER_HOST="tcp://192.168.99.100:2376"
export DOCKER_CERT_PATH="/Users/xxxxx/.docker/machine/machines/default"
export DOCKER_MACHINE_NAME="default"
# Run this command to configure your shell:
# eval "$(docker-machine env default)"
//}

この場合は、@<code>{DOCKER_HOST}環境変数に見えている192.168.99.100がアクセスするべきIPアドレスとなります。http://192.168.99.100:8080/info.phpにアクセスしてください。

（Windowsの場合も同じでいい？）

Linuxの場合は同じホストで実行されるので、localhostや127.0.0.1になります。

PHPの情報が表示されれば、環境の準備は成功しています。
（Docker Toolbox使っている場合はlocalhostじゃなさそう）

「Not Found」の画面が表示されてしまう場合は、どこかで間違えてしまったようです。これまでの手順を見ながらやり直してみてください。

@<code>{docker run}をやり直すには、以下のコマンドを実行して一旦Dockerコンテナを停止し、削除する必要があります。

//emlist[Dockerコンテナの停止と削除]{
% docker stop pdfsearch
% docker rm pdfsearch
//}

ありがちな間違いとして、Dockerコンテナとの共有ディレクトリーの指定ミスがあります。@<code>{docker run}（@<code>{docker create}）コマンドの@<code>{volume}オプションをもう一度確認しましょう。@<code>{$PWD}は（@<code>{docker run}を実行した）現在のディレクトリーを意味しますので、そこと別のディレクトリーにファイルを置いた場合は、コンテナ内のPHPが認識できません。事前にプロジェクトのディレクトリーに移動してから実行するようにしましょう。

//footnote[share-volume][原則を外れることもできますが、本書の範囲外のため解説しません。気になる方はDockerの公式サイトなどで@<code>{volume}オプション及び「Dockerfileの@<code>{VOLUME}命令」を調べてみてください。]
