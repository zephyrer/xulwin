# Title:
# Create Release Build
#
# Purpose:
# Creation of the API bundle that is easy to deploy for end-users.
# API bundle = Headers + Libs
#
# Args:
# ARGV[0] Root dir
# 
#
# Algorithm:
# Perform SVN status to check for changes, if changes are detected then report error
# Perform SVN update to make sure we get the correct revision number when calling svn info
# Perform SVN info and remember the revision number
# Create a release folder named after the revision number in the Libs directory
# Create vs80 and vs90 subfolders
# Copy vs80 LuaBindings.lib into the vs80 release folder
# Copy vs90 LuaBindings.lib into the vs90 release folder
# Copy vs80 XULWin.lib into the vs80 release folder
# Copy vs90 XULWin.lib into the vs90 release folder
# Compress the release folder
# End of script
require 'ftools'
require 'fileutils'

projectroot = ARGV[0]
puts "projectroot: " + projectroot
curdir = Dir.pwd
puts "curdir: " + curdir
Dir.chdir(projectroot)

# Perform SVN status to check for changes in the project root dir, if changes
# are detected then report error
system("svn st > svn-state")
File.readlines("svn-state").each do |line|
	if (!line.empty? && line[0] == 'M') then
		raise "SVN modifications found!"
	end
end


# Perform SVN update to make sure we get the correct revision number when calling svn info
system("svn update")

# Perform SVN info and remember the revision number
system("svn info > svn-info")
rev = ""
File.readlines("svn-info").each do |info|
	if info =~ /^Revision\: (\d+)/ then
		rev = $1
	end
end

if rev.empty?
	raise "Obtaining the SVN revision number has failed."
end

# Create a release folder named after the revision number in the Libs directory
releasedir = "#{projectroot}/Libs/XULWin_rev#{rev}"
puts releasedir
FileUtils.mkdir_p(releasedir)

# Create vs80 and vs90 subfolders
FileUtils.mkdir_p("#{releasedir}/vs80")
FileUtils.mkdir_p("#{releasedir}/vs90")

# Copy vs80 LuaBindings.lib into the vs80 release folder (same for vs90)
File.copy("#{projectroot}/../Output/Windows/vs80/Debug/LuaBindings/LuaBindings.lib", "#{releasedir}/vs80")
File.copy("#{projectroot}/../Output/Windows/vs90/Debug/LuaBindings/LuaBindings.lib", "#{releasedir}/vs90")

# Copy vs80 XULWin.lib into the vs80 release folder (same for vs90)
File.copy("#{projectroot}/../Output/Windows/vs80/Debug/XULWin/XULWin.lib", "#{releasedir}/vs80")
File.copy("#{projectroot}/../Output/Windows/vs90/Debug/XULWin/XULWin.lib", "#{releasedir}/vs90")

# Compress the release folder
Dir.chdir(releasedir)
system("7z a XULWin_rev#{rev}")


# End of script
puts "Done!"