; Code by Valentin-Gabriel Radu, license is GPLv3
; Faculty of Automatic Control and Computer Science
; University Politehnica of Bucharest
; References: Another great video by Tushar, same
; as in KMP: https://www.youtube.com/watch?v=H4VrKHVG5qI&t=669s
;
; Assemble, link and run using:
; nasm -g -f elf32 rk.asm -o rk.o
; gcc -g -m32 rk.o -o rk
; ./rk
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
extern strncmp

%define SIZEOF_DWORD 4

section .data
    maxlength equ 100000;
    pattern dd 0;
    text times maxlength db 0;
    text_length dd 0;
    pattern_store times maxlength db 0;
    pattern_length dd 0;
    powers dd 1, 3, 9, 27, 81;
    format_d db '%d', 10, 0;
    power dd 3;
    begin_match db 27, '[1;32m', 0 ; bright green colour for text terminal
    end_match db 27, '[0m', 0 ; restore default colour
    format db '%s', 0

section .text

initial_hash:
    ; compute full hash for text
    push ebp
    mov ebp, esp
    mov esi, [ebp + 8]  ; text
    mov ebx, [ebp + 12] ; length
    
    xor ecx, ecx
    loop_pattern_hash:
    xor eax, eax
    mov al, byte [esi + ebx - 1]
    mul dword [powers + (ebx - 1) * SIZEOF_DWORD]
    add ecx, eax
    dec ebx
    jnz loop_pattern_hash
    
    mov eax, ecx
    
    leave
    ret

main:
    push ebp
    mov ebp, esp; for correct debugging
    
    mov eax, [ebp + 8] ; argc
    mov ecx, [ebp + 12] ; argv

    cmp eax, 1 ; see if args list contains more than one argument (program's executable path)
    jz exit
    mov ecx, [ecx + 4] ; get content of first argument
    push ecx ; push it to stack
    call strlen ; find length of it
    mov [pattern_length], eax ; save length
    pop ecx ; get address of first argument
    mov [pattern], ecx ; and save it
    
    push dword [stdin]
    push dword maxlength
    push dword text
    call fgets
    call strlen
    mov [text_length], eax
    add esp, 4
    
    ;push dword [stdin]
    ;push dword maxlength
    ;push dword pattern_store
    ;call fgets
    ;call strlen
    ;dec eax
    ;mov [pattern_length], eax
    ;add esp, 4
    ;mov [pattern_store + eax], byte 0 ; remove trailing enter off patern
    ;mov dword [pattern], pattern_store
    
    push dword [pattern_length]
    push dword [pattern]
    call initial_hash
    add esp, 8
    
    push eax
    
    push dword [pattern_length]
    push text
    call initial_hash
    add esp, 8
    
    pop ecx
    ; eax -> hash of first pattern_length chars of text
    ; ecx -> hash of pattern
    
    mov edi, [text_length] ; length
    ;sub edi, [pattern_length]
    inc edi
    xor esi, esi ; counter
    xor edx, edx ; print in colour
    main_loop:
    cmp eax, ecx
    jnz check
    ; we have a match
    
    push eax
    push ebx
    push ecx
    push esi
    push edi
    push edx
    mov eax, text
    add eax, esi
    push dword [pattern_length]
    push eax
    push dword [pattern]
    call strncmp
    add esp, 12
    pop edx
    test eax, eax
    jnz finalize
    mov edx, [pattern_length]
    finalize:
    pop edi
    pop esi
    pop ecx
    pop ebx
    pop eax
    
    check:
    test edx, edx
    jz else
    push eax
    push ebx
    push ecx
    push edx
    push edi
    push esi
    
    push begin_match
    push format
    call printf
    add esp, 8
    pop esi
    push esi
    xor eax, eax
    mov al, byte [text + esi]
    push eax
    call putchar
    add esp, 4
    push end_match
    push format
    call printf
    add esp, 8
    
    pop esi
    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax
    
    dec edx
    jmp continue
    else:
    push eax
    push ebx
    push ecx
    push edx
    push edi
    push esi
    xor eax, eax
    mov al, byte [text + esi]
    push eax
    call putchar
    add esp, 4
    pop esi
    pop edi
    pop edx
    pop ecx
    pop ebx
    pop eax
    continue:
    push edx
    xor edx, edx
    mov dl, byte [text + esi]
    sub eax, edx
    xor edx, edx
    div dword [power]
    mov ebx, eax
    xor eax, eax
    mov edx, [pattern_length]
    mov al, byte [text + esi + edx]
    mul dword [powers + (edx - 1) * SIZEOF_DWORD]
    add ebx, eax
    mov eax, ebx
    inc esi
    pop edx
    cmp esi, edi
    jnz main_loop
    
    
    exit:
    
    leave
    xor eax, eax
    ret