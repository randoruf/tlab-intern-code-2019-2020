clear;  
%% Use HCTSA 
x = 1:10; 
try 
    x = ST_Length(x); 
catch 
    hctsa_startup_path = fullfile('..', '..', '..', '..', 'startup.m');
    run(hctsa_startup_path)
end
clear x hctsa_startup_file 

%% KL divergence of two epsilon machines 

% time series 1 
indata1 = '2444333333444444444221111111111222222111111111111111444331112244442224222224444444333311222224444444444444444444444441111111111111111111111333333111111111133311111133334444444444444444211111224444444441114444444444444431133334444222111111111334442222442111111333333122222111113331122222222223333344444444444444433333331122222111133333444444444311111111344444421111111111133444211133333311111222222211111111134444444222222111111244444442111111111244444444444422222244331122444444444431221124444444444444442111111111111111111111111111112444444441111333333444444422222111111111111112442224422222111134444444444211111111112222222221111133333111122244444444443333333311244422444444444442111111111111122244444444333111111111111111114433333111111144444442221133344444444444444444444433333311344311111111111133222222212222222222222244444444311111111222242222111111111111111222222223311111334444444433333443311111224444444444444444444444444331111111112244433333311344444444444444433311111114444111111111111112222221111111111111112222442224333311111111113333444444433333111111111111112244444444444444444444444444444444443311111111111122222222222211113333333331111112222222222244433333311224444444444443111111111111111111112444444444331111333444444444444443333334444444222222222243333331111111111111122222221113333444444444422221111134442224444442113333344444444444333333333333122221111111111111111122444444222211111111112222224444444333334444444444444444433333311111111122444111133311133311113311111311111111222222222222444444433344444233331112211111111112244444443311111444444444433331111111112244444444444411111111111122244444444444211111111113444444444444444444431111111111111111111111111111112222222111111333344442111111334444444444433331112222444344444222211111133344444444222222222113333334444444422221111111111333331222111111111344444444444442222444444444433333333344444444211111111111111111111111111111222244444444444444444444442111111111111122222211133334444444433333333333333342222211333311111122224444422444222444422222222222244444444444444444433111111113333111111111111111111111111111111111111111111144444444444442222222224444333333334444444444433111111111113444444444444444444444444444444111222211112222222111444444422211111122224444444444333311111112222224444443111111111111111111111111113333111111122222221134444444444444444444444444444444444444444444444444444444444331111111333333333111111111111111111111111111111111111111111111111111222222211111122443111243331224444444433444444444444444444444444444444444444422222222222444444111111111111111111113311111111134444433311333333333333333331111113344333333333111111111111111122224442222222222224444444444444444224444444444443444411111111111111111111333331222222222222113333333333334444433311111111111133333344443311122442224444444444442222111122222111122244444442111344211111111333333444444443111111112222222244443333444444444411111113311112222243334421111112444444222222222111133334333333333111111224444444444111224444444444441111111333443333333333311133331111111211121111111333333111112444444444442222222443333333333333333333342244444422222444442222211111111111111111111442111111111133333344441111111244444444444444333443334443311244433333333333111112224444443333311111111111111111122222221111111111111114444442222211444444444444444444422224444444443333311333444444444444433333444444444433331111111111111112222222111111111111111111333444444444444444444444444433111111111111111113333444444444444433422224444221111111111122244444444444444222111224444422444444431111111222224444433331111114444444443333311111111111111113331111111333333444444444444444444444444442222244444222222224444444444444444431111111111111111113111111111111334444444433333444444444444221113333333333344444444444444444444443333333331111221111111111111111111111122244444444444444111222211333311111111113444333444444444443331111112222222211111112222211111111111222222211222444444444444444444444333343333333333311111111111111111112222244443111111111222222444222222444444444333333344'; 
indata1 = char(indata1 + 16);

% time series 2 
indata2 = '1111122222444444444221111113444444444444444444444444444422111333333333311111112222222222222222244444444444441111113333333333333111111111111111111111111111111112444444444444422222111111112222222111111111112222222222222444444443334444444444444444444444444433333333333444444444444444444444433333331111111111111111111111111111111111111111111111111111111111111111111133344444433333333334444444444444444444444444444444444444444444444444444422211111111111111111111111111111111111111111111111111111111111111111111111111111111244444444444422222222244444444444222222224444433333333333333334444444444444444444444444444444444444444444444422222222222222222222222222111111111111111111111111111111111111111333333333333333333333333333333333333333333333333333333333333344444444444444444444422222222222222222222222222222222222222221111111111111111113333333333333333333333333333333333333333333344444444444444444444443334444444444444442111111111122222222222222111111111111111111111111113333311111111111111111111111111222222224444444444444444444444444444444444444444444444444311111111111333333333333111133333333333333333333111111111111111111111111111111113334444444444444444444444444442222222222222222222222222222444444444422222211111111333331111111111111111111333333333311111111111113111111111111111111111111111112222222444444444444444444444444444444444444444422222222222222222222111222222221111111111111111111111111111111111111111333333333333333331222222222222244444444444444444444111111333334444444444444422211111133344444444444444444444444444444422222221111111111111111111111111111122444444444221111111111111222244444444421111111111113344444444444442211111111222244444444442111111111111124224444444433111111111111111111111111113333333344444444444444222111111112244444444444444444433333333333344422222221111111222222222222444444222222111111111111333333333333333111111111111111111111111111111111111111113333111133333333333344444444444444444444444444442222222222224444442222222222222222222222211111111111111111111111111111111333334444443111111111133333333333111111111111344444444444444211144444444444444444444444444444444444444444422222222222222111111111111111111111111133333444444444443334444444444444444444442222222222222222111111111111111111111111111111111111113344444444444444444444444444444444444444444444444444444444444444444444444333334444333333331111111111111111111111111111111111111111111111111111111111111111111222222222111111111111111111111113333333333333333333344444444444444444444444444444444444444444444444444444444444444444444444431111111111111111111111111111111111111111111111111111111111133333333111111111111111133334444224444444444444444444444444444444444444444444444444444444444444444433333333333311111111111111111111111111111111111111111111111111111111133344444444444444221111112222222222222244444444444444444444444444444444444444444444444444444444111111111111111111111111111111111111111111111133344444444442244444444444444444444444444444442222222222222244444444444222111111111111113333333333333333111111111111111111111111111111111111111111344444444444444444444444444433333333111111344444444444444444444444444444444444444444444222222211111134444421111111111111111111111111111111111111111111111111111111111112222222244444444444444444444444444444444444444444444422222222222224444444443333111111111111111111111111111111333333333333333333333333444444444444444444444444444444444444444444444444444444444444444222221111111111111112224444422222111111111111111333444444211111111111111111111333333333333334444424444444444444444444444444444444444444422333111112222221111111111111122222111111111111111222222222222222244444444444444111111111111444444444442222111144444333311111111113344444444444442111111111111111111111111111222224444444444444444444443333333333333331133334444444444444444444444444222222222244443333344442222222222111111111111111111111111111111111111111111111113444444444444444444444444444444444444444444444333111111112444222244433333333333333111111111111111111111111111111224444444444444222222222';
indata2 = char(indata2 + 16);


% parameters of CSSR  
alphabet = 'ABCD'; 
history_len = 6; 
sigma_level = 0.001; 

% calculate by CSSR
[~, ~, ipi_p, tmp_file1] = CSSR(alphabet, indata1, history_len, sigma_level); 
[~, ~, ipi_q, tmp_file2] = CSSR(alphabet, indata1, history_len, sigma_level); 

% get KL divergence 
base = 'E'; 
initial_digit = 'A'; 
[kl_p_q, kl_q_p] = KL_divergence_two_epsilon_machine( ...
                        history_len + 1, base, initial_digit, ... 
                        ipi_p, ipi_q, ...
                        [tmp_file1 '_inf(2).txt'], [tmp_file2 '_inf(2).txt'])
                    

