import sys
import os
import json



def handle_unspt_flags(cmd):
    for each in unspt_flags:
        index = cmd.find(each)
        if index != -1:
            cmd = cmd[:index] + cmd[index+len(each):]
    return cmd

def bc_phase(org_command):
    global bcfiles
    cmd = org_command['command']
    cmd = handle_unspt_flags(cmd)
    dir = org_command['directory']
    file = org_command['file']
    filename = file[len(dir)+1:]
    obj = filename[:-1] + 'o '
    index = cmd.find(obj)
    if index == -1:
        obj = filename[:-2] + " "
        index = cmd.find(obj)
        if index == -1:
            print "command error;%s\n"%file #>>logfile
            return
        print "command warning;%s\n"%file #>>logfile
    bcfiles.append(filename[:-1] + 'bc')
    cmd = cmd[0:index] + filename[:-1] + 'bc ' + cmd[index+len(obj):]
    new_command = "clang -emit-llvm" + cmd[2:]
    return new_command

def opt_phase(org_command):
    pass

def link_phase(org_command):
    pass

logfile = None
#unsupport compiler flags
unspt_flags = ["-mno-fp-ret-in-387",
               "-mpreferred-stack-boundary=3",
               "-mskip-rax-setup",
               "-mindirect-branch=thunk-extern",
               "-mindirect-branch-register",
               "-fno-var-tracking-assignments",
               "-fconserve-stack",
               "-mpreferred-stack-boundary=2",
               "-Wstrict-aliasing=3",
               "-mindirect-branch=thunk-inline"
               ]
bcfiles = []
if __name__ == "__main__":
    global logfile, bcfiles
    logfile = open("linux-bc.log", 'a+')
    if len(sys.argv) != 2:
        print "argv is errir"
        exit()
    bear_json = sys.argv[1]
    fp = open(bear_json)
    out_fp = open(os.path.dirname(bear_json)+"/command.sh", 'w')
    bear = json.load(fp, encoding="utf8")
    command_list = bear#.JSONDecoder()
    out_command = []
    for each in command_list:
        new_cmd = bc_phase(each)
        if new_cmd: 
            out_command.append(new_cmd)
#        out_command.append(opt_phase(each) )
#        out_command.append(link_phase(each))
    out_fp.write("set -x"+os.linesep)
    out_fp.writelines(os.linesep.join(out_command))
    out_fp.write("\n llvm-link-9 %s >linuxall.bc \n" % " ".join(bcfiles))
    bclist_fp = open("bc.list", 'w')
    bcfiles.sort()
    bclist_fp.write(" ".join(bcfiles))
    bclist_fp.close()
    out_fp.close()
    fp.close()
    logfile.close()
