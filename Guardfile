require 'pathname'

ignore(/\/?\.\#/)

SRC = Pathname.new('src')
DEST = Pathname.new('web')

guard :shell do
  watch %r|#{SRC}/(.+\.re)| do |m|
    src = m[0]
    dest = m[1]
    `review-preproc #{src} > #{dest}`
  end

  watch %r|(.+)\.re| do |m|
    src = m[0]
    next if src.start_with? "#{SRC}/"
    dest = (DEST/m[1]).sub_ext('.html')
    dest.dirname.mkpath unless dest.dirname.exist?

    `review-compile --target html #{src} > #{dest}`
  end
end

guard :livereload do
  watch %r|#{DEST}/.+|
end
