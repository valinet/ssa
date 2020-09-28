; Code by Valentin-Gabriel Radu, license is GPLv3
; Faculty of Automatic Control and Computer Science
; University Politehnica of Bucharest
; References: 
; [1] https://ocw.cs.pub.ro/courses/iocla
; [2] http://www.nasm.us/doc/nasmdoc3.html
; [3] https://stackoverflow.com/questions/8006711/whats-the-difference-between-equ-and-db-in-nasm
; [4] https://stackoverflow.com/questions/13064809/the-point-of-test-eax-eax
; [5] http://asm.sourceforge.net/intro/hello.html
; [6] https://en.wikipedia.org/wiki/Data_segment
; [7] https://www.youtube.com/watch?v=GTJr8OvyEVQ
; [8] https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
; Assemble, link and run using:
; nasm -g -f elf32 kmp.asm -o kmp.o
; gcc -g -m32 kmp.o -o kmp
; ./kmp
; Example test (courtesy of fortune utility):
; $ Type text:
; Kindness is a language which the deaf can hear and the blind can read. - Mark Twain
; $ Type pattern: can

global main
extern printf
extern fflush
extern stdout
extern putchar
extern fgets
extern stdin
extern strlen
extern strcpy

%define SIZEOF_DWORD 4

section .data
    maxlength equ 100000 ; max length of sentence & pattern, increase as required
    ; idea: [8]
    begin_match db 27, '[1;32m', 0 ; bright green colour for text terminal
    end_match db 27, '[0m', 0 ; restore default colour
    format db '%s', 0 ; string print format for printf
    text times maxlength db 0 ; buffer containing text to be matched
    size dd 0 ; size of text buffer
    offset dd 0 ; offset required for printing from where we left last time
    pattern dd 0;
    pattern_store times maxlength db 0 ; buffer conatining pattern to match
    length dd 0 ; size of pattern buffer
    output times maxlength dd 0 ; size of position pattern buffer, same as pattern buffer size
    typetext db 'Type text:', 10, 0
    typepattern db 'Type pattern: ', 27, '[1;32m', 0
    welcome db 'Welcome! This program will perform KMP algorithm on ', 27, '[1;36m', \
    'text', 27, '[0m', ' that you will', 10, 'supply, and try to find a matching ', 27, '[1;36m', \
    'pattern', 27, '[0m', ' that you will supply as well.', 10, \
    'Code by Valentin-Gabriel Radu. Read source file for acknowledgments.', 10, 0

section .text

print:
    push ebp
    mov ebp, esp
    
    ; esi is match_end position, relative to start of original string (sentence)
    ; from the way the algorithm works, it is i + 1
    inc esi
    
    ; edi is match_start
    ; it is i - strlen(pattern); for efficiency, we do here edi - esi because we
    ; already have what is required in edi, and then we just negate and add 1
    sub edi, esi
    not edi
    inc edi
    
    ; import a pointer to original sentence, the value of offset, and a pointer to
    ; the string containing the format for printf from memory
    mov ecx, text
    add ecx, [offset]
    mov edx, format

    ; save start letter, as we temprarly replace it with \0 for printf
    mov al, byte [text + edi]
    xor ebx, ebx
    mov [text + edi], bl
    push eax
    push ecx
    push edx
    call printf
    ; one can optionally flush the buffers and force the output to show at stdout
    ; immediatly; otherwise, you have to wait until the application closes, control
    ; to kernel is returned and buffers are flushed; the former is useful for debugging
    ;
    ; push dword [stdout]
    ; call fflush
    ; add esp, 4
    pop edx
    pop ecx
    pop eax
    ; restore original letter to string
    mov byte [text + edi], al
    
    ; adjust pointer to string
    add ecx, edi
    sub ecx, [offset]
    
    ; write the ANSI escape code so that the colour of the text terminal changes; save
    ; relevant registers, as function calls destroy them
    push ecx
    push edx
    push begin_match
    call printf
    add esp, 4
    pop edx
    pop ecx
    
    ; save end letter
    mov al, byte [text + esi]
    xor ebx, ebx
    mov [text + esi], bl
    push eax
    push ecx
    push edx
    call printf
    ; could flush stdout
    ;
    ; push dword [stdout]
    ; call fflush
    ; add esp, 4
    add esp, 4
    pop ecx
    pop eax
    mov byte [text + esi], al
    
    ; write current position to offset, so next time we hit this block we do not 
    ; write the whole sentence once again
    mov [offset], esi
    
    ; write ANSI escape code that restores default colour to text terminal
    push end_match
    call printf
    add esp, 4
    
    leave
    ret

main:
    push ebp
    mov ebp, esp; for correct debugging
    
    ; type welcome text
    ;;;push welcome
    ;;;call printf
    ;;;add esp, 4
    
    ; type first command text
    ;;;push typetext
    ;;;call printf
    ;;;add esp, 4
    

    mov eax, [ebp + 8] ; argc
    mov ecx, [ebp + 12] ; argv

    cmp eax, 1 ; see if args list contains more than one argument (program's executable path)
    jz exit
    mov ecx, [ecx + 4] ; get content of first argument
    push ecx ; push it to stack
    call strlen ; find length of it
    mov [length], eax ; save length
    pop ecx ; get address of first argument
    mov [pattern], ecx ; and save it
    
    ; read sentence
    push dword [stdin] ; read from stdin
    push maxlength ; max length of buffer
    push text ; pointer to memory location where sentence should be read
    call fgets ; read function call
    call strlen ; strlen will use the previous pointer as argument (still on stack)
    mov [size], eax ; save output from strlen
    add esp, 4 ; remove buffer pointer from stack
    
    ; type second command text
    ;;;push typepattern
    ;;;call printf
    ;;;add esp, 4
    
    ;push pattern_store ; pointer to memory location where pattern should be read
    ;call fgets ; read function call
    ;call strlen ; get length of this string as well
    ;mov [length], eax ; save it
    ;add esp, 12 ; discard stdin, maxlength and buffer pointer from stack
    
    ;mov esi, pattern_store
    ;mov [pattern], esi
    
    ; check if pattern contains a trailing CR, and remove it
    ; as that will prevent correct pattern matching; eax still contains
    ; length of pattern from last function call
    ;mov ebx, [esi + eax - 1]
    ;cmp ebx, 10
    ;jnz program ; begin execution if not the case
    ;mov [esi + eax - 1], byte 0 ; set last byte to \0 instead of \10
    ; decrease length as well
    ;mov eax, [length]
    ;dec eax
    ;mov [length], eax
    
    ; restore original text terminal colour
    ;;;push end_match
    ;;;call printf
    ;;;add esp, 4
    
    program:
    
    ; program implements KMP algorithm as described at:
    ; [7]
    
    ; precompute position vector for KMP
    ; this part was originally written as in the video, and
    ; then instructions were rearranged for better assembly
    ; readability and optimization based on the following C code:
    ;   while (i < strlens)
    ;   {
    ;		if (s[j] - s[i])
    ;		{
    ;			if (j > 0)
    ;			{
    ;				j = (*out)[j - 1];
    ;				continue;
    ;			}
    ;			else j--;
    ;		}
    ;		j++;
    ;		(*out)[i] = j;
    ;		i++;
    ;	}
    ;
    ; originating from:
    ;
    ; 	while (i < strlens)
    ;	{
    ;		if (s[j] - s[i] && j > 0)
    ;		{
    ;			j = (*out)[j - 1];
    ;			continue;
    ;		}
    ;		else if (s[j] - s[i] && j == 0)
    ;		{
    ;			(*out)[i] = j;
    ;			i++;
    ;		}
    ;		else
    ;		{
    ;			j++;
    ;			(*out)[i] = j;
    ;			i++;
    ;		}
    ;	}
    
    ; i = 1
    xor esi, esi
    inc esi
    ; j = 0
    xor edx, edx
    ; set ecx to length and edi to &pattern for faster comparison
    mov ecx, [length]
    mov edi, [pattern]
    while_i_smaller_length:
    ; compute pattern[j]
    mov al, byte [edi + edx]
    ; compute pattern[i]
    mov bl, byte [edi + esi]
    cmp al, bl
    jz increment_j
    test edx, edx ; cmp edx, 0
    jz finalize_iteration
    mov edx, [output + edx * SIZEOF_DWORD - 1 * SIZEOF_DWORD]
    jmp while_i_smaller_length
    increment_j:
    inc edx
    finalize_iteration:
    mov [output + esi * SIZEOF_DWORD], edx
    inc esi
    cmp esi, ecx
    jnz while_i_smaller_length
    
    ; end of precompute
    
    ; begin of actual algorithm
    ; this is based on the following C code modeled after what is
    ; described in the video:
    ;   for (i = 0; i < strlens; i++)
    ;	{
    ;		if (s[i] == p[j])
    ;		{
    ;			j++;
    ;			if (j == strlenp)
    ;			{
    ;				printf("%d %d\n", i - strlenp + 1, i + 1);
    ;				j = 0;
    ;			}
    ;		}
    ;		else
    ;		{
    ;			if (j != 0)
    ;			{
    ;				j = out[j - 1];
    ;				i--;
    ;			}
    ;		}
    ;	}

    ; i = 1
    xor esi, esi
    ; j = 0
    xor edx, edx
    ; set ecx to size for faster comparison
    mov ecx, size
    ; set edi to length for faster comparison
    mov edi, [length]
    jmp for_i_zero_to_size
    loop_increment:
    inc esi
    cmp esi, [size]
    jz end
    for_i_zero_to_size:
    ; build pattern[j]
    mov eax, [pattern]
    mov bl, byte [eax + edx]
    xor eax, eax
    ; build text[i]
    mov al, byte [text + esi]
    cmp eax, ebx
    jnz else
    inc edx
    cmp edx, edi
    jnz loop_increment
    
    ; in order for the algorithm to continue working correctly, I save all the
    ; regsiters in use to stack
    push eax
    push ebx
    push ecx
    push esi
    push edi
    
    ; subroutine that prints the output; we print the part of the sentence
    ; prior to the start position of the match, then change color to bright green
    ; output the matched word, and save the ending position of the match so
    ; that next time we can print from that address; standard printf calls are
    ; used to print data; in order to avoid allocating and copying data from
    ; the original text, which is slower, we save and replace the characters on
    ; match_start and match_end positions with \0, and restore them later
    ; (printf stops output at \0)    
    call print
    
    ; print subroutine is done, restoring original registers contents
    pop edi
    pop esi
    pop ecx
    pop ebx
    pop eax
    ; end of print subroutine
    xor edx, edx
    jmp loop_increment
    else:
    test edx, edx ; cmp edx, 0
    jz loop_increment
    mov edx, [output + edx * SIZEOF_DWORD - 1 * SIZEOF_DWORD]
    jmp for_i_zero_to_size
    
    end:
    ; print the rest of the sentence until the end of it
    mov edx, text
    add edx, [offset]
    push edx
    push format
    call printf
    add esp, 8
    ; always be careful to restore stack to original position
    
    exit:
    xor eax, eax
    leave
    ret
    
    ; original code to print position vector
;    mov ecx, format
;    mov edx, length
;    not edx
;    inc edx
;    print:
;    xor eax, eax
;    mov eax, [output + length * SIZEOF_DWORD + edx * SIZEOF_DWORD]
;    
;    push edx
;    push eax
;    push ecx
;    call printf
;    pop ecx
;    add esp, 4
;    pop edx
;    
;    inc edx
;    jnz print


;    push eax
;    push ebx
;    push ecx
;    push esi
;    push edi
;    inc esi
;    push esi
;    sub esi, edi
;    inc esi
;    push esi
;    mov ecx, format
;    push ecx
;    call printf
;    add esp, 12
;    pop edi
;    pop esi
;    pop ecx
;    pop ebx
;    pop eax