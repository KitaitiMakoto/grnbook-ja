Eye.application 'mysqld' do
  process :mysqld do
    pid_file '/var/run/mysqld/mysqld.pid'
    start_command 'mysqld'
    stop_signals 'mysqladmin -uroot shutdown'
    daemonize true
  end
end
