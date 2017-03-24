import sys, math

multiplier=2

def main(file_list):

	sample_name = file_list[0][file_list[0].find("DataCards/")+10:file_list[0].find("/", file_list[0].find("DataCards/")+10)]
	print("Buildind x%d cards for %s" %(multiplier, sample_name))
	for f in file_list:
		f_in = open(f, "r")
		f_out = open(f.replace(sample_name, "%s_x%d" % (sample_name, multiplier)), "w+")
		for line in f_in:			
			toks = line.split()
			if (toks[0] == "observation") or  (toks[0] == "rate"):
				"""Observations need to be multiplied by the multiplier"""
				l2=toks[0]+"   "+toks[1]+"  "
				for t in toks[2:]:
					if t != '-':
						l2+="%f     " % float(t)*multiplier
					else:
						l2+="%s     " % t
				f_out.write(l2)
			elif "fsbkg_stat_syst" in toks[0]:
				"""FS Stat needs count to be multiplied by the multiplier"""
				l2=toks[0]+"   "+toks[1]+"  "
				l2+="%d      " % math.floor(int(toks[2])*multiplier)
				for t in toks[3:]:
					l2+="%s     " % t
				f_out.write(l2)
			elif "stat_syst" in toks[0]:
				"""The rest of the log normal stat uncs need to be divided by the sqrt(multiplier)"""
				l2=toks[0]+"   "+toks[1]+"  "
				for t in toks[2:]:
					if t != '-':
						l2+="%f     " % float(t)/sqrt(multiplier)
					else:
						l2+="%s     " % t
				f_out.write(l2)
			else:
				f_out.write(line)



if __name__ == "__main__":
	main(sys.argv)