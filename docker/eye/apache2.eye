Eye.application 'web' do
  process :apache2 do
    pid_file '/var/run/apache2/apache2.pid'
    start_command '/usr/sbin/apache2ctl -D FOREGROUND'
  end
end
