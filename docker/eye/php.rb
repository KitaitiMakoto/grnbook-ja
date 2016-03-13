Eye.application 'web' do
  working_dir '/var/lib/pdfsearch'
  process :php do
    pid_file 'php.pid'
    start_command 'php -S 0.0.0.0:80'
    daemonize true
  end
end
