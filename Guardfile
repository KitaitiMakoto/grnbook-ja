require 'pathname'

ignore(/\/?\.\#/)

DEST = Pathname.new('web')

guard :shell do
  watch %r|(.+)\.re| do |m|
    src = m[0]
    dest = (DEST/m[1]).sub_ext('.html')
    dest.dirname.mkpath unless dest.dirname.exist?

    `review-compile --target html #{src} > #{dest}`
  end
end

guard :livereload do
  watch %r|#{DEST}/.+|
end
