# README

University of Washington

TCES 460

Winter 2019


# Development Team Vulcan
@authors

Name             | Email
---------------- | -----------------------
Alex Boyle       | boylea4@uw.edu
Ammon Dodson     | ammon0@uw.edu
Alex Marlow      | alexmarlow117@gmail.com
Jake McKenzie    | jake314@uw.edu
Brooke Stevenson | brooks04@uw.edu

@copyright Copyright &copy; 2019 by the authors. All rights reserved.

# Project Pages
*	[The Github Repo] (https://github.com/ammon0/Syringenator)
*	[Documentation Website] (https://ammon0.github.io/Syringenator/index.html)

# Using Git
Git is a command-line tool for managing source code. Github is an on-line service that provides git remotes. A git remote is a remote copy of a git repository. Multiple people work in the same repository through the use of a single remote. The trick is to manage version conflicts intelligently.

Each team member should periodically merge master into their own branch to ensure that we are synced up. The master branch should only ever have merge commits and working code. I will try to enforce this with Github so that we don't make a mess.
--ABD

## Work in Your Own Branch
Each team member should create their own branch to work in. You may make as many branches as you like, just make sure you have one. You can create branches on the command line with:
```
$ git branch <branch-name>
```
To switch to your branch do:
```
$ git checkout <branch-name>
```

## Commit Your Work
Commits are a permanent record of your work. They should be as small and purpose-driven as possible. Think: "can I write a couple lines that explains what I did?" To check for uncommitted changes, or check your status in general do:
```
$ git status
On branch ammon
Your branch is up-to-date with 'github/ammon'.	<- this is the remote
Changes not staged for commit:
  (use "git add/rm <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   README.md

Untracked files:
  (use "git add <file>..." to include in what will be committed)

	docs/autotoc_md6.html
	latex/autotoc_md6.tex

no changes added to commit (use "git add" and/or "git commit -a")

```
You make a commit in two steps: first you stage the changed files that you want to include in this next commit.
```
$ git add <filename> <anotherfile>
```
Once you have staged a bunch of changes you can check your status again:
```
$ git status
On branch ammon
Your branch is up-to-date with 'github/ammon'.
Changes to be committed:
  (use "git reset HEAD <file>..." to unstage)

	modified:   README.md

Changes not staged for commit:
  (use "git add/rm <file>..." to update what will be committed)
  (use "git checkout -- <file>..." to discard changes in working directory)

	modified:   Makefile
	deleted:    refman.pdf

Untracked files:
  (use "git add <file>..." to include in what will be committed)

	docs/autotoc_md7.html
	latex/autotoc_md7.tex
```
Once you are satisfied with what is currently staged you finish the commit by doing:
```
$ git commit
```
Git will automatically open a text editor where you can describe what the changes are. Make this a meaningful message since it will be the only thing that distinguishes this commit from hundreds of others.

You can also do:
```
$ git commit -m "<commit message>"
```
(-m is shorthand for --messages command which tells other collaborators (and your future self) the nature of the change you just made.
--Jake

## Merge All the Latest Changes
The magic of git is being able to merge conflicting changes. Before you share your changes (pushing), you must pull the latest changes and merge them with yours. First pull the master branch:
```
$ git pull origin master
```
You will need to enter your password and git will tell you if there have been any changes. Git will attempt to merge the master branch into yours. If there are any conflicts it will tell you. Git will rewrite your files to include both versions of the conflicting code. To see which files are in conflict do:
```
$ git status
```
You have to open those files, find, and fix the conflicting versions. Once you think you are done, rebuild and test all the code. Look for any new errors and fix them. Once you are satisfied that the merge has been completed successfully add and commit your changes as usual.

## Push Your Branch
Pushing your work to the remote allows everyone else to see it. You should merge master before pushing. To push do:
```
$ git push origin <your-branch>
```

# HypoRobot Assignment
If you’ve been paying any attention at all to current events you know that a major plague has descended on cities and counties throughout the country in the form of used and discarded hypodermic needles. Countless hours are spent cleaning up this mess. For instance, some schools are forced, for safety reasons, to send staff out to scour the playgrounds prior to children showing up.

Your task this quarter will be to design an autonomous robot that can help automate the arduous and sometimes dangerous job of spotting, retrieving, and disposing of hypodermic syringes.

Your robot will be a prototype, not a fully functional disposal robot, but it will have important technical features necessary on such a robot.

A second point is that we will be dealing with industrial (i.e. dull) syringes. These are typically used to disburse such things glue or solvents. They are commonly used in our labs to glue acrylic parts together. Anyone in the lab with a sharp needle will be immediately disqualified. Even so, if you would rather not design and test with any syringe, you may, with my written permission, use a ballpoint pen, a #2 pencil or a similar object of your choosing.

All testing will be done indoors on a flat surface.

--Robert Gutmann, Ph.D.
