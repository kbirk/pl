# Vagrantfile API/syntax version. Don't touch unless you know what you're doing!
VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
    # Every Vagrant virtual environment requires a box to build off of.
    config.vm.box = "ubuntu/trusty64"

    # sync folder for workbench dir hierarchies
    config.vm.synced_folder "./", "/home/vagrant/pl/", create: true

    # See http://www.thisprogrammingthing.com/2013/getting-started-with-vagrant/
    config.vm.provision :shell, :path => "provision.sh"
end
