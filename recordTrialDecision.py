from datetime import datetime

def getDateTimeString(dateObject):
    dateStrComponent = list();
    dateStrComponent.append(str(dateObject.year));
    dateStrComponent.append(str(dateObject.month));
    dateStrComponent.append(str(dateObject.day));
    dateStrComponent.append(str(dateObject.hour));
    dateStrComponent.append(str(dateObject.minute));

    return '_'.join(dateStrComponent);

file_name = raw_input('Input subject name\n');
now = datetime.now();
file_name = file_name + "_" + getDateTimeString(now);

no_trial = raw_input('Input number of trials\n');
current_trial = 1;
with open(file_name,'w') as f:
    while(current_trial <= int(no_trial)):
        decision = raw_input('Input decision, 1st or 2nd is heavier? [input either 1 or 2]\n');
        f.write(decision + "\n"); #trial decision
        current_trial += 1
f.close();

print "Trial finished, program terminating";
