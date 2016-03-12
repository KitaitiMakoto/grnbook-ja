Eye.application 'web' do
  working_dir '/var/lib/grnbook'
  process :php do
    pid_file 'php.pid'
    start_command 'php -S 0.0.0.0:80'
  end
end