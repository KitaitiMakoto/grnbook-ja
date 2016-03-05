require 'yaml'
require 'pathname'

ignore(/\/?\.\#/)

SRC = Pathname.new('src')
DEST = Pathname.new('web')

catalog = YAML.load_file('catalog.yml')
CHAPS = catalog['CHAPS']

guard :shell do
  watch %r{#{SRC}/(.+)\.re|(ch\d+)?/.+} do |m|
    chap = m[1] || m[2]
    `rake #{chap}`
  end

  watch %r{\Aimages/.+\.(?:png|jpe?g|gif|webp)} do |m|
    src = m[0]
    dest = DEST/src
    dest.dirname.mkpath unless dest.dirname.exist?

    `cp #{src} #{dest}`
  end
end

guard :livereload do
  watch %r|#{DEST}/.+|
end
