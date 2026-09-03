#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//2. Você deve implementar uma tabela de chamada de pacientes em um consultório médico. 
//A tabela deve ser apresentada no início e apenas as linhas necessárias devem ser atualizadas (pesquise sobre sequências de escape ANSI).
//Cada linha é independente, ou seja, linhas diferentes podem ser atualizadas ao mesmo tempo.
//Além disso, deve haver exclusão mútua por linha, logo, uma thread só pode modificar uma linha seela estiver livre. 
//Uma linha deve ficar alguns segundos bloqueada antes de ser modificada novamente para que a  mudança seja visível. 
//As informações sobre novos pacientes devem ser lidas de arquivos de texto externos (você deve criá-los). 
//Neles, devem ser informados os nomes dos pacientes e o número do consultório. Haverá N arquivos com L linhas cada


int main() {
    
    return 0;
}