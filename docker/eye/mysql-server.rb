Eye.application 'db' do
  process :mysqld do
    pid_file 'mysqld.pid'
    start_command 'mysqld_safe'
  end
end
