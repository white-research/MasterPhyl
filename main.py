# -*- coding: utf-8 -*-
"""
Created on Mon Jul 13 11:54:48 2015

@author: dominic
"""
import decimal

def parse_tnt(filename):
    f = open(filename,'r')
    
    ### GET TOP DATA
    l1 = f.readline().strip()
    if l1 != 'xread':
        raise Exception("Tnt file must start with xread")
    print l1.strip()
    comment = ''
    is_comment = False
    while True:
        ct_line = f.readline()
        if is_comment == False:
            if ct_line[0] == "'":
                is_comment=True
                comment=comment+ct_line.replace('\n',' ')
            elif ct_line[0] == '"':
                raise Exception("Initial comment should use single quotations: '")
        else:
            if "'" in ct_line:
                comment=comment+ct_line.replace('\n',' ')
                ct_line = f.readline()
                break
            else:
                comment=comment+ct_line.replace('\n',' ')
    ct_line = ct_line.split()
    print ct_line
    if len(ct_line)!=2:
        raise Exception("You must have a line containing two integers:\
        characters and taxa")
    try:
        nchar = int(ct_line[0])
        ntaxa = int(ct_line[1])
    except ValueError:
        raise Exception("You must have a line containing two integers:\
        characters and taxa")
    matrix = {'name':comment,'ntaxa':ntaxa,'nchar':nchar, 'taxon_names':[]}
    
    ### GET CHARACTER DATA
    found_end_line = False
    partitions = []
    partition_taxa = []
    partition_chars = {}
    partition_datatype = ''
    for l in f.readlines(): ### go through each line
        print "This line is:", l.strip()
        
        if l[0]==';':  ### if line is end of matrix
            assert_partition_correct(ntaxa, matrix['taxon_names'], partition_taxa, partition_datatype, partition_chars)
            matrix['taxon_names'] = partition_taxa.sort()
            partitions.append({'datatype':partition_datatype,
                               'chars':partition_chars,
                              })
            found_end_line = True
            break
        
        elif l[0]=='&':  ### if line is new character type
            print '\n'
            if len(partitions) > 0 or len(partition_taxa) > 0:
                assert_partition_correct(ntaxa, matrix['taxon_names'], partition_taxa, partition_datatype, partition_chars)
                matrix['taxon_names'] = partition_taxa.sort()
                partitions.append({'datatype':partition_datatype,
                                   'chars':partition_chars,
                                  })
            partition_taxa = []
            partition_chars = {}
            partition_datatype = l[l.find('[')+1:l.find(']')]
            
        else:  ### if line contains data
            name_end = l.find(' ')
            taxon_name = l[:name_end]
            partition_taxa.append(taxon_name)
            character_states = l[name_end+1:].strip()
            if partition_datatype == []:
                partition_datatype = 'num'
            
            ### parse depending on datatype
            if partition_datatype == 'num':
                parsed_states = []
                i = 0
                while i < len(character_states):
                    if character_states[i] == '[':
                        end = character_states.find(']',i)
                        range_of_states = character_states[i+1:end]
                        if len(range_of_states) > 2:
                            raise Exception('There is a polymorphism with more than two states. States must be between 0 and 9 inclusive')
                        elif len(range_of_states) == 1:
                            parsed_states.append(int(range_of_states[0]))
                        else:
                            parsed_states.append([int(range_of_states[0]),int(range_of_states[1])])
                        i = end
                    elif character_states[i] == '?':
                        parsed_states.append(character_states[i])
                    else:
                        parsed_states.append(int(character_states[i]))
                    i = i+1
                print parsed_states
            
            elif partition_datatype == 'cont':
                parsed_states = []
                i = 0
                while i < len(character_states):
                    print character_states[i:], len(character_states)
                    next_break = character_states.find(' ', i)
                    print next_break
                    if next_break == -1:
                        cont_states = character_states[i:]
                        i = len(character_states)
                    else:
                        cont_states = character_states[i:next_break]
                        i = i+next_break+1
                    if '-' in cont_states:
                        cont_states=cont_states.split('-')
                        for idx, state in enumerate(cont_states):
                            cont_states[idx]=decimal.Decimal(state)
                    elif cont_states != '?':
                        cont_states = decimal.Decimal(cont_states)
                    print i
                    parsed_states.append(cont_states)
                print parsed_states
            
            elif partition_datatype == 'serial':
                parsed_states = []
                i = 0
                while i < len(character_states):
                    if character_states[i]=='[':
                        parsed_seg = []
                        seg_end = character_states.find(']',i)
                        seg_states = character_states[i+1:seg_end]
                        print '  ',seg_states
                        j = 0
                        while j < len(seg_states):
                            c_end = seg_states.find(' ',j)
                            print '   ',c_end
                            if c_end == -1:
                                range_of_state = seg_states[j:]
                                if range_of_state != '?':
                                    try:
                                        range_of_state = int(range_of_state)
                                    except ValueError:
                                        range_of_state = decimal.Decimal(range_of_state)
                                parsed_seg.append(range_of_state)
                                break
                            else:
                                range_of_state = seg_states[j:c_end]
                                if range_of_state != '?':
                                    try:
                                        range_of_state = int(range_of_state)
                                    except ValueError:
                                        range_of_state = decimal.Decimal(range_of_state)
                                parsed_seg.append(range_of_state)
                                j=c_end+1
                        i = seg_end
                        parsed_states.append(parsed_seg)
                    i = i+1
                print parsed_states
            
            else:
                raise Exception("Unknown datatype")
            
            ### add parsed states from that line to the character matrix
            partition_chars[taxon_name] = parsed_states 
    
    if found_end_line != True:
        raise Exception("Character data needs to end with a ';' symbol on a new line")
    f.close()
    matrix['partitions'] = partitions
    assert_matrix_correct(matrix)
    return matrix


def assert_partition_correct(ntaxa, existing_taxa, partition_taxa, partition_datatype, partition_chars):
    if len(partition_taxa)==0:
        raise Exception("There seems to be an error with the number of taxa")
    if len(partition_taxa)!=ntaxa:
        print partition_taxa
        raise Exception('Wrong number of taxa in partition')
    if existing_taxa != [] and existing_taxa != partition_taxa.sort():
        raise Exception('Taxa do not match exisitng')
    
    ### go through and make sure all data matches for partition
    if partition_datatype == 'num':
        init_len = len(partition_chars[partition_taxa[0]])
        print partition_chars
        for t in partition_taxa:
            assert(len(partition_chars[t])==init_len)
            print partition_chars[t]
            for c in partition_chars[t]:
                if type(c)!=list:
                    assert(type(c)==int or c=='?')
                    if type(c)==int:
                        assert(c>=0 and c<=9)
                else:  ### if polymorphism
                    assert(len(c)==2)
                    assert(c[0]<c[1])
                    for p in c:
                        assert(type(p)==int or p=='?')
                        if type(p)==int:
                            assert(p>=0 and p<=9)
    elif partition_datatype == 'cont':
        init_len = len(partition_chars[partition_taxa[0]])
        print partition_chars
        for t in partition_taxa:
            assert(len(partition_chars[t])==init_len)
            print partition_chars[t]
            for c in partition_chars[t]:
                if type(c)!=list:
                    assert(type(c)==decimal.Decimal or c=='?')
                else:  ### if polymorphism
                    assert(len(c)==2)
                    assert(c[0]<c[1])
                    for p in c:
                        assert(type(p)==decimal.Decimal or p=='?')
    elif partition_datatype == 'serial':
        init_seg_len = len(partition_chars[partition_taxa[0]][0])
        char_types = []
        for c in partition_chars[partition_taxa[0]][0]:
            if c=='?':
                char_types.append(c)
            else:
                char_types.append(type(c))
        for t in partition_taxa:
            for s in partition_chars[t]:
                assert(len(s)==init_seg_len)
                print char_types
                print s
                for idx, c in enumerate(s):
                    if char_types[idx] == '?':
                        if c != '?':
                            char_types[idx]=type(c)
                    else:
                        if c!='?':
                            assert(type(c)==char_types[idx])
    else:
        raise Exception("Unknown datatype")
    return


def assert_matrix_correct(matrix):
    return

mat = parse_tnt('input')
print '\n\n\n'
for x in mat.keys():
    if x == 'partitions':
        for y in mat[x]:
            print '\n',y['datatype']
            print y['chars']
    else:
        print '\n',x, mat[x]
































