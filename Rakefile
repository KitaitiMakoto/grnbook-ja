require 'rake/clean'
require 'yaml'
require 'pathname'

SRC_DIR = Pathname.new('src')
WEB_DIR = Pathname.new('web')
directory SRC_DIR.to_path
directory WEB_DIR.to_path

catalog = YAML.load_file('catalog.yml')
CHAPS = FileList[catalog['CHAPS']]
SRCS = FileList[CHAPS.map {|chap| SRC_DIR/chap}]
WEBS = SRCS.ext('html').collect {|web| web.gsub(SRC_DIR.to_path, WEB_DIR.to_path)}

CLEAN.include CHAPS
CLOBBER.include WEBS

task :default => :book

desc 'Build files of the book'
task :book => [:epub, :pdf]

desc 'Build EPUB file'
task :epub => CHAPS.ext('') do
  sh "review-epubmaker config.yml"
end

desc 'Build PDF file'
task :pdf => CHAPS.ext('') do
  sh "review-pdfmaker config.yml"
end

desc 'Build all HTML files'
task :html => WEBS

CHAPS.each do |chap|
  html = WEB_DIR/chap.ext('html')

  name = chap.ext('')
  assets = FileList["#{name}/**/*"]

  desc "Build chapter #{name}"
  task name => html

  file html => [chap, WEB_DIR.to_path] do |task|
    sh "review-compile --yaml=config.yml --target=html --output-file=#{task.name} #{task.prerequisites.first}"
  end

  file chap => SRC_DIR/chap do |task|
    sh "review-preproc #{task.prerequisites.first} > #{task.name}"
  end

  unless assets.empty?
    file chap => assets

    # Exists to notify updates of timestamp
    task assets
  end
end
