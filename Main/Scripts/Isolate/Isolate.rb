require "rexml/document"
#
# Usage
# ruby Isolate.rb
#
# For example:
# ruby Isolate.rb Label
#
# Generates LabelElement.h, LabelElement.cpp, Label.h and Label.cpp
# and adds these files to the Visual Studio project.
#

def compile_template(template_file, output_file, mapping)
    File.open(template_file, 'r') do |template|
        File.open(output_file, 'w') do |out|
            while (line = template.gets())
                new_line = line
                mapping.each do |key, value|
                    new_line = new_line.gsub(key, value)
                end
                out << new_line
            end
        end
    end
	return output_file
end


def compile_element_header_file(template_header_file, element_name, component_name, hpp_path)
    compile_template(template_header_file,
                     File.join(hpp_path, element_name + ".h"),
                     { "{{ELEMENT_NAME}}" => element_name,
                       "{{ELEMENT_NAME_UPPER}}" => element_name.upcase(),
                       "{{ELEMENT_TYPE}}" => component_name.downcase() })
end


def compile_element_source_file(template_source_file, element_name, component_name, cpp_path)
    compile_template(template_source_file,
                     File.join(cpp_path, element_name + ".cpp"),
                     { "{{ELEMENT_NAME}}" => element_name,
                       "{{COMPONENT_NAME}}" => component_name })
end


def compile_component_header_file(template_header_file, component_name, hpp_path)
    compile_template(template_header_file,
                     File.join(hpp_path, component_name + ".h"),
                     { "{{COMPONENT_NAME}}" => component_name,
                       "{{COMPONENT_NAME_UPPER}}" => component_name.upcase() } )
end


def compile_component_source_file(template_source_file, component_name, cpp_path)
    compile_template(template_source_file,
                     File.join(cpp_path, component_name + ".cpp"),
                     { "{{COMPONENT_NAME}}" => component_name } )
end


def add_file_to_vs_project(filter_node, relative_path)
	element = REXML::Element.new("File")
	element.attributes["RelativePath"] = relative_path
	filter_node.add_element(element)
	puts "Filter node: #{filter_node.to_s}. Path: #{relative_path.to_s}."
end


def add_files_to_vs_project(project_path,
							element_hpp_file,
							element_cpp_file,
							component_hpp_file,
							component_cpp_file)
	contents = ""

	File.open(project_path, 'r') do |file|
		doc = REXML::Document.new file
		doc.get_elements("VisualStudioProject/Files").each do |root_filter|
			root_filter.each_element("Filter[@Name='Elements']") do |elements_filter|
				elements_filter.each_element("Filter[@Name='Header Files']") do |element_headers_filter|
					add_file_to_vs_project(element_headers_filter, ".\\include\\XULWin\\" + element_hpp_file)
				end
				elements_filter.each_element("Filter[@Name='Source Files']") do |element_sources_filter|
					add_file_to_vs_project(element_sources_filter, ".\\src\\" + element_cpp_file)
				end
			end
			
			root_filter.each_element("Filter[@Name='Components']") do |components_filter|
				components_filter.each_element("Filter[@Name='Header Files']") do |component_headers_filter|
					add_file_to_vs_project(component_headers_filter, ".\\include\\XULWin\\" + component_hpp_file)
				end
				components_filter.each_element("Filter[@Name='Source Files']") do |component_sources_filter|
					add_file_to_vs_project(component_sources_filter, ".\\src\\" + component_cpp_file)
				end
			end
		end

		contents << doc.to_s
		
		# Prevent Visual Studio Conversion Wizard trigger
		contents.sub!(/'8,00'/, "\"8,00\"")
		contents.sub!(/'9,00'/, "\"9,00\"")
	end
	
	File.open(project_path, 'w') do |file|
		file << contents
	end
end


class CppObject
	attr_accessor :class_name, :header_body, :cpp_methods
	def initialize(class_name, header_body)
		@class_name = class_name
		@header_body = header_body
		@cpp_methods = []
	end
	
	def addMethod(method)
		@cpp_methods.push(method)
	end
end


def extract_element_declarations(file)
	result = []
	lines = File.new(file, "r").readlines()
	in_class = false
	class_body = ""
	class_name = ""
	lines.each do |line|
		if not in_class		
			if line =~ /    class (\w+Element)/
				class_name = $1
				in_class = true
			end			
		end
		
		if in_class
			class_body += line
			if line =~ /[ ][ ][ ][ ]\}\;/			
				result.push(CppObject.new(class_name, class_body))
				class_body = ""
				class_name = ""
				in_class = false				
			end
		end
	end
	return result
end


def extract_element_definitions(class_name, file)
	result = []
	lines = File.new(file, "r").readlines()
	in_method = false	
	header_body = ""
	lines.each do |line|
		if not in_method		
			if line =~ /[ ][ ][ ][ ](\w+)::.+/
				if $1.to_s.eql? class_name
					in_method = true
				end
			end
		end
		
		if in_method
			header_body += line
			if line =~ /[ ][ ][ ][ ]\}/
				result.push(header_body)
				header_body = ""
				in_method = false
			end
		end
	end
	return result
end


def main
	project_path = "../../XULWin/"
	hpp_path = File.join(project_path, "include/XULWin/")
	cpp_path = File.join(project_path, "src/")
	
	el_decls = extract_element_declarations(File.join(hpp_path, "Element.h"))
	el_decls.each do |cppobject|
		el_def = extract_element_definitions(cppobject.class_name, File.join(cpp_path, "Element.cpp"))
		el_def.each do |method|
			cppobject.addMethod(method)
		end
		puts "Number of cpp_methods for #{cppobject.class_name} is #{cppobject.cpp_methods.length}."
	end
	
	i = 0
	while i < 3
		el_decls.each do |decl|			
			puts decl.header_body
			decl.cpp_methods.each do |method|
				puts method
				puts ""
			end
		end
		i += 1
	end
	
	# save_element_header(File.join(project_path, "include/XULWin/Element.h"), el_decl)
	# save_element_source(File.join(project_path, "include/XULWin/#{
	
	
	# element_hpp_file = compile_element_header_file("ElementTemplate.h", element_name, component_name, hpp_path)
	# element_cpp_file = compile_element_source_file("ElementTemplate.cpp", element_name, component_name, cpp_path)
    # component_hpp_file = compile_component_header_file("ComponentTemplate.h", component_name, hpp_path)
    # component_cpp_file = compile_component_source_file("ComponentTemplate.cpp", component_name, cpp_path)

	# system("svn add #{element_hpp_file}")
	# system("svn add #{element_cpp_file}")
	# system("svn add #{component_hpp_file}")
	# system("svn add #{component_cpp_file}")
		
	# add_files_to_vs_project(File.join(project_path, "XULWin_vs90.vcproj"),
							# File.basename(element_hpp_file),
							# File.basename(element_cpp_file),
							# File.basename(component_hpp_file),
							# File.basename(component_cpp_file))
end

main()




