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

  file chap => [assets, SRC_DIR/chap] do |task|
    sh "review-preproc #{task.prerequisites.first} > #{task.name}"
  end

  # Exists to notify updates of timestamp
  task assets unless assets.empty?
end
