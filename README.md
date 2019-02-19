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
*	[The Github Repo](https://github.com/ammon0/Syringenator)
*	[Documentation Website](https://ammon0.github.io/Syringenator/index.html)

# Communication
## Don't Clobber Other People's Work
Since we're all working in the same space it is important to be courteous. Pretty much this comes down to not overwriting other people's work. If there is some real need to change something that already exists there should be a discussion between everyone involved.

## Comment Your Work
Not everything will be obvious to everyone else. Write a paragraph for every non-trivial function. Write a detailed explanation any time you want to get clever with the code. Always put your name or initials on larger comments and blocks of code that you have written. That way it's easy to know who to talk to if there are questions.

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

## Make a Pull Request
The master branch is where we integrate all the changes everyone is making. This is done through "pull requests". A pull request is a way for everyone to see and comment on new code. It will also allow us to only make merge commits to the master branch. If we work this way the master branch will always be clean and there will be less errors, lost work, and wasted time.

## What not to do
*	**Don't commit directly to master**. I've tried to setup Github to make this difficult or impossible, but in any case that it isn't protected properly nobody should be trying this anyway.
*	**Don't --force** Read your error messages, they are usually very helpful. The force tag overwrites history and can easily erase work already done. If git complains there is a reason for it.

# HypoRobot Assignment

@author Robert Gutmann, Ph.D.

If you’ve been paying any attention at all to current events you know that a major plague has descended on cities and counties throughout the country in the form of used and discarded hypodermic needles. Countless hours are spent cleaning up this mess. For instance, some schools are forced, for safety reasons, to send staff out to scour the playgrounds prior to children showing up.

Your task this quarter will be to design an autonomous robot that can help automate the arduous and sometimes dangerous job of spotting, retrieving, and disposing of hypodermic syringes.

Your robot will be a prototype, not a fully functional disposal robot, but it will have important technical features necessary on such a robot.

A second point is that we will be dealing with industrial (i.e. dull) syringes. These are typically used to disburse such things glue or solvents. They are commonly used in our labs to glue acrylic parts together. Anyone in the lab with a sharp needle will be immediately disqualified. Even so, if you would rather not design and test with any syringe, you may, with my written permission, use a ballpoint pen, a #2 pencil or a similar object of your choosing.

All testing will be done indoors on a flat surface.

## Terminology

The following terms are used in this specification:
*	The term “autonomous”, in this case, means that no commands can be transmitted to your robot from any outside agency (especially from a human or computer or other controller) and all sensors used in the contest must be physically attached to your robot. No wired connections are allowed between any outside agency and your robot.
*	The term “course” refers to the area in which the contest takes place.
*	The term “tape line” refers to an oval of white tape that runs from a start point around the oval, back to the start point (which is now the finish point). All targets will be placed outside of the oval.
*	The term “target” refers to the object you are required to pick up and dispose of (syringe or, alternatively, a pen or pencil).
*	The term “decoy” refers an object on the course that is not a target. A decoy will be less than 2 cm tall.
*	The term “obstacle” refers to an object on the course that your robot must avoid running into. An obstacle will be at least 15 cm tall. A typical obstacle would be a cardboard box.
*	The term “finish the course” will mean that your robot traverses the oval at least once.
Note: Your robot will have to leave the tape line to pick up targets, but it should eventually either find another target or return to the tape line. The tape line is your navigation aid.
*	The term “contact a target” will mean to touch a target with your pick-up mechanism in such a way as to move it. Note: moving a target with a robot wheel or track does not count as a contact.
*	The term “participate” will mean that you either finish the course or contact a target.
*	The term “acquire a target” means your robot has reported to its data logger that it has identified a target and reports an accurate position for that target. The term “acquire a decoy” means your robot has reported to its data logger that it has acquired a target that turns out to be a decoy.
*	The term “pick up a target” refers to your robot picking up a target off the course surface.
*	The term “dispose of a target” refers to your robot placing the target in container on your robot.
*	A robot is “stationary” if its wheels are not rotating and its arm is not rotating about its vertical axis.

## Rules of the Game

*	You will be given two test runs, one per day over two class periods. The dates will be firmly established by midterm time.
*	All tests will be conducted indoors.
*	A somewhat different course may be laid out each day. The layout will consist of:
	*	A tape line; this will serve as your navigation maker. Since we will be indoors, we won’t have GPS; the tape line will serve as your navigation reference.
	*	A number of targets will be placed within 1 meter of the tape line; you will have to leave the tape line to pick up your targets.
	*	A number of decoys will be placed within 1 meter of the tape line.
	*	A number of obstacles will be placed on the course. If you exactly follow the tape line you will not run into an obstacle; however, you may have to avoid obstacles as you maneuver away from the tape line to pick up targets.
*	No human will be allowed on the course during a test run.
*	Your robot must be autonomous.
*	All test runs will be video ‘taped.’
*	The goal is to maximize your score according to the algorithm discussed below. The maximum score you achieve for any one day over the two days will be your final score.
*	The scores for the entire class will be rank-ordered.
*	You will be allowed ten minutes on the course for each test run. This will be strictly timed.
*	Robot
	*	You will be provided with
		*	A basic robot chassis
		*	Two motors with encoders and wheels
		*	Two motor controllers (H-bridges)
		*	A robotic arm
		*	A battery pack with a power distribution unit
		*	Distance sensors.
		*	Line sensors
		*	Data logger with SD card
	*	You do not have to use this robot chassis or arm
	*	You will need to supply your own processor(s)
	*	You will need to supply your own cameras(s) and cables.
	*	You may acquire additional mechanical or electronic parts for your robot.
	*	If you plan to spend any money on your robot, you must get permission from me in writing first.
	*	Your group has a strict budget of $300, including any parts that you have already acquired and use on your robot (e.g., an Arduino).
*	Rule 8 applies. Rule 8 comes from the official rules for the annual Race to Alaska (see https://r2ak.com/official-rules/). Rule 8 states, and I quote: If we decide it’s necessary to consult a lawyer to figure out if you are disqualified or not, you are automatically disqualified. Play by the rules and live up to the spirit of the race. If you get cute and push the boundaries, we’ll bring down the hammer.
