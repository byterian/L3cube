/*
	Problem Definition: To list duplicate files from hard drive
	
	Author: 
		BYTErians
    	Group No.28
	
	Assumptions:
		1) Duplicate files are identified on the basis of size, last modified date and extension of the file.
		2) Delete operation will remove all the duplicate files within a specified directory/drive/hard disk.
		
	Idea:
		A list of all the files in a directory/drive is created using recursion(to handle sub directories).
		Meta data(size, date of last modification, extension) of file is added to the set which is a collection of distinct elements.
		If Meta data already exists in the Set then it is marked as a duplicate file.
		A list of all the duplicate files in specified directory/drive is created and user is given an option to remove them.
		
	Usage:
		Compile: javac DuplicateFiles.java	
		Execute: java DuplicateFiles <Directory Name>
		
*/

import java.io.*;
import java.io.IOException;
import java.util.*;

class DuplicateFiles
{
	/*
		This function creates an ArrayList of all the files in a specified directory.
		It uses recursion to handle sub directories.
	*/
	static void createContents(File f, ArrayList<File> allFiles)
	{
		int i;
		File fList[] = f.listFiles();             // fetches list of all the files and directories in the current directory
                
		try
		{
			for (i=0; i < fList.length; i++) 
			{
				if (fList[i].isFile())            // returns a boolean indicating isFile status
				{
					allFiles.add(fList[i]);
				} 
				else if (fList[i].isDirectory())   // returns a boolean indicating isDirectory status
				{
					createContents(fList[i], allFiles);         // recursive call to handle sub directories
				}
			}
		}	
		catch(Exception ex)
		{}
	}
	
	/*
		This function finds duplicate files by adding unique meta data(size, data last modified, extension) about the file 
		in a set(collection of distinct elements). If the meta data already exists in the set then the file is a duplicate file.
	*/
	static void findDuplicateFiles(ArrayList<File> allFiles,ArrayList<File> duplicateFiles)
	{
		 Set<String> Data = new HashSet<String>();
		 String s;
		 File f;
		 int i;
		 
		 System.out.println("\nList of duplicate files:\n");
		 
		 for(i=0; i < allFiles.size(); i++)
		 {
			f = allFiles.get(i);
			Date d = new Date(f.lastModified());          // fetches last modified date attribute of the file
			
			s = f.getName();
			
			if(!Data.contains(f.length() + " " + d.toString() + " " + s.substring(s.lastIndexOf(".") + 1)))    // if meta data is unique, it is added to the set
				Data.add(f.length() + " " + d.toString() + " " + s.substring(s.lastIndexOf(".") + 1));
			else                                       // if meta data is not unique the file under consideration is a duplicate file
			{
				System.out.println(f.getPath() + "\n");     // Print duplicate file path
				duplicateFiles.add(f);				 // Add duplicate file to ArrayList of duplicate files
			}
			try
			{
				Thread.sleep(10);
			}
			catch(InterruptedException ex)
			{} 
		}
	}
	
	/*
		This function removes the duplicate files from the specified directory if the user wishes to.
	*/
	
	static void removeDuplicateFiles(ArrayList<File> duplicateFiles)
	{
		String s;
		int i;
		System.out.print("\nDo you want to delete all the duplicate files(yes/no) ::");
		Scanner in = new Scanner(System.in);
		s = in.nextLine();			// scan user choice
		if(s.equals("yes"))			// if user wishes to remove duplicate files
		{
			for(i=0; i < duplicateFiles.size(); i++)
			{
				if(duplicateFiles.get(i).delete())
					System.out.println("\n" + duplicateFiles.get(i).getPath() + " deleted");
				else
					System.out.println("Delete operation failed");
			}
		}
	}
	
	public static void main(String args[])
	{
		try
		{
			String x = args[0];
			File f = new File(x);    // a File object representing x
			int i;
			
			ArrayList<File> allFiles = new ArrayList<File>();
			ArrayList<File> duplicateFiles = new ArrayList<File>();
			 
			if(f.exists())     //returns a boolean indicating exists status for a file/directory
			{
				if(f.isDirectory())     //returns a boolean indicating isDirectory status
				{
					createContents(f,allFiles); 
					
					findDuplicateFiles(allFiles,duplicateFiles);
					
					if(duplicateFiles.size() != 0)
						removeDuplicateFiles(duplicateFiles);
				}
				else if(f.isFile())     //returns a boolean indicating isFile status
				{
					System.out.println(x + " is a file. Please enter a directory name");
				}
				else
				{
					System.out.println(x + " is neither a directory nor a file");
				}
			}
			else
			{
				System.out.println(x + " does not exists");
			}
		}
		catch(ArrayIndexOutOfBoundsException ex)
		{
			System.out.println("Usage: java DuplicateFiles <Directory Name>");
		}
	}
}