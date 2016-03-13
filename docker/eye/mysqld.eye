Eye.application 'mysqld' do
  process :mysqld do
    pid_file '/var/run/mysqld/mysqld.pid'
    start_command 'mysqld_safe'
  end
end
