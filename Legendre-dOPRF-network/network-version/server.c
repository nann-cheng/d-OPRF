// server.c

#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../arith.h"
#include "../dOPRF.h"
#include "../network.h"

#define PORT 9080

/////////////////////////////////////////////
// Trusted party setup
// Provide common shared strings
// Can also be done via PKI
/////////////////////////////////////////////
void mal_tp_setup(prng_seed seed, RSS_i k_i[CONST_N][LAMBDA], RSS_seed_i rseed_i[CONST_N], DRSS_seed_i dseed_i[CONST_N], ASS_seed_zero_i aseedz_i[CONST_N], RSS_seed_zero_i rseedz_i[CONST_N], DRSS_seed_zero_i dseedz_i[CONST_N]){
    /////////////////////////////////////////////
    // Random RSS key k (LAMBDA many)
    /////////////////////////////////////////////
    RSS (*k) = malloc(sizeof(RSS[LAMBDA]));
    RSS_i (*k_i_temp)[CONST_N] = malloc(sizeof(RSS_i[LAMBDA][CONST_N]));
    RSS_rand_array(k, seed);


    for(int j = 0; j < LAMBDA; j++)
        distribute_RSS(k[j], k_i_temp[j]);

    for(int i = 0; i < CONST_N; i++)
        for(int j = 0; j < LAMBDA; j++)
            memcpy(k_i[i][j], k_i_temp[j][i], sizeof(RSS_i));

    /////////////////////////////////////////////
    free(k);
    free(k_i_temp);

    /////////////////////////////////////////////
    // Seeds for generating random RSS & DRSS shares 
    /////////////////////////////////////////////
    
    // RSS
    RSS_seed (*rseed) = malloc(sizeof(RSS_seed));
    // RSS_seed_i (*rseed_i) = malloc(sizeof(RSS_seed_i[CONST_N]));

    random_RSS_seed(*rseed, seed);
    distribute_RSS_seed(*rseed, rseed_i);
    free(rseed);


    // DRSS
    DRSS_seed (*dseed) = malloc(sizeof(DRSS_seed));
    // DRSS_seed_i (*dseed_i) = malloc(sizeof(DRSS_seed_i[CONST_N]));

    random_DRSS_seed(*dseed, seed);
    distribute_DRSS_seed(*dseed, dseed_i);
    free(dseed);
    /////////////////////////////////////////////


    /////////////////////////////////////////////
    // Seeds for generating ASS, RSS & DRSS secret shares of zero
    /////////////////////////////////////////////
    // ASS
    ASS_seed_zero (*aseedz) = malloc(sizeof(ASS_seed_zero));
    // ASS_seed_zero_i (*aseedz_i) = malloc(sizeof(ASS_seed_zero_i[CONST_N]));

    random_ASS_seed_zero(*aseedz, seed);
    distribute_ASS_seed_zero(*aseedz, aseedz_i);
    free(aseedz);


    // RSS
    RSS_seed_zero (*rseedz) = malloc(sizeof(RSS_seed_zero));
    // RSS_seed_zero_i (*rseedz_i) = malloc(sizeof(RSS_seed_zero_i[CONST_N]));

    random_RSS_seed_zero(*rseedz, seed);
    distribute_RSS_seed_zero(*rseedz, rseedz_i);
    free(rseedz);


    // DRSS
    DRSS_seed_zero (*dseedz) = malloc(sizeof(DRSS_seed_zero));
    // DRSS_seed_zero_i (*dseedz_i) = malloc(sizeof(DRSS_seed_zero_i[CONST_N]));

    random_DRSS_seed_zero(*dseedz, seed);
    distribute_DRSS_seed_zero(*dseedz, dseedz_i);
    free(dseedz);
    /////////////////////////////////////////////



    
    // free(k_i);

    // free(rseed_i);
    // free(dseed_i);

    // free(aseedz_i);
    // free(rseedz_i);
    // free(dseedz_i);
}

/////////////////////////////////////////////
// Generate secret shares of zero to be used in online part of protocol
/////////////////////////////////////////////
void mal_setup_zeros(const int i, ASS_seed_zero_i aseedz_i, RSS_seed_zero_i rseedz_i, DRSS_seed_zero_i dseedz_i, ASS_i az_i[LAMBDA][TAU_i * TAU_i][3], RSS_i rz_i[LAMBDA], DRSS_i dz_i[LAMBDA]){

    /////////////////////////////////////////////
    // Generate all the ASS shares of zero
    /////////////////////////////////////////////
    f_elm_t (*az_temp)[3 * LAMBDA] = malloc(sizeof(f_elm_t[TAU_i * TAU_i][3 * LAMBDA]));

    ind_T T_ind = 0;
    for(share_T T0 = T_0; T0.ind < TAU; next_T(&T0)){
    for(share_T T1 = T_0; T1.ind < TAU; next_T(&T1)){
        if(i_hold_TT(i, T0, T1)){
            subset_S S; subset_holding_TT(T0, T1, &S);
            ASS_zero_local_array(S, i, 3 * LAMBDA, az_temp[T_ind++], aseedz_i);
        }
    }}

    T_ind = 0;
    for(share_T T0 = T_0; T0.ind < TAU; next_T(&T0)){
    for(share_T T1 = T_0; T1.ind < TAU; next_T(&T1)){
        if(i_hold_TT(i, T0, T1)){
            for(int j = 0; j < LAMBDA; j++)
                memcpy(az_i[j][T_ind], az_temp[T_ind][3*j], sizeof(f_elm_t[3]));
            T_ind++;
        }
    }}
    /////////////////////////////////////////////
    free(az_temp);



    /////////////////////////////////////////////
    // Generate one RSS share of zero
    /////////////////////////////////////////////
    // f_elm_t (*rz_i)[TAU] = malloc(sizeof(f_elm_t[LAMBDA][TAU_i]));
    f_elm_t (*rz_temp)[LAMBDA] = malloc(sizeof(f_elm_t[TAU_i][LAMBDA]));
    RSS_zero_local_array(i, LAMBDA, rz_temp, rseedz_i);
    
    T_ind = 0;
    for(share_T T = T_0; T.ind < TAU; next_T(&T)){
        if(i_hold_T(i, T)){
            for(int j = 0; j < LAMBDA; j++)
                memcpy(rz_i[j][T_ind], rz_temp[T_ind][j], sizeof(f_elm_t));
            T_ind++;
        }
    }
    /////////////////////////////////////////////
    free(rz_temp);


    /////////////////////////////////////////////
    // Generate one RSS share of zero
    /////////////////////////////////////////////
    f_elm_t (*dz_temp)[LAMBDA] = malloc(sizeof(f_elm_t[TAU_i * TAU_i][LAMBDA]));
    DRSS_zero_local_array(i, LAMBDA, dz_temp, dseedz_i);

    T_ind = 0;
    for(share_T T0 = T_0; T0.ind < TAU; next_T(&T0)){
    for(share_T T1 = T_0; T1.ind < TAU; next_T(&T1)){
        if(i_hold_TT(i, T0, T1)){
            for(int j = 0; j < LAMBDA; j++)
                memcpy(dz_i[j][T_ind], dz_temp[T_ind][j], sizeof(f_elm_t));
            T_ind++;
        }
    }}
    /////////////////////////////////////////////
    free(dz_temp);
}



/////////////////////////////////////////////
// First phase - Computing the product
// Parties compute first part of local RSS->RSS mult and send outputs
/////////////////////////////////////////////
void mal_setup_mul_1(const RSS_i rz_i[LAMBDA], RSS_i c_i[LAMBDA][CONST_N][TAU_i * TAU_i], RSS_seed_i rseed_i, DRSS_seed_i dseed_i){

    /////////////////////////////////////////////
    // Compute the input of the multiplication
    // A random RSS element s, and another one s+0
    /////////////////////////////////////////////
    RSS_i (*s)[LAMBDA] = malloc(sizeof(RSS_i[2][LAMBDA]));

    random_RSS_local_array(LAMBDA, s[0], rseed_i);
    for(int l = 0; l < LAMBDA; l++)
        add_RSS_i(s[0][l], rz_i[l], s[1][l]);


    /////////////////////////////////////////////
    // Compute the square
    /////////////////////////////////////////////
    RSS (*c)[TAU_i * TAU_i] = malloc(sizeof(RSS[LAMBDA][TAU_i * TAU_i]));

    for(int l = 0; l < LAMBDA; l++)
        mult_RSS_to_RSS_evaluation(s[0][l], s[1][l], c[l], dseed_i);

    /////////////////////////////////////////////
    free(s);

    /////////////////////////////////////////////
    // Party computes an (many) RSS share
    // Send shares T_j to party j
    // Re_indexing of these outputs
    /////////////////////////////////////////////
    for(int l = 0; l < LAMBDA; l++){
        for(int j = 0; j < CONST_N; j++){
            for(share_T T0 = T_0; T0.ind < TAU_i; next_T(&T0)){
            for(share_T T1 = T_0; T1.ind < TAU_i; next_T(&T1)){
                ind_T T_ind = 0;
                for(share_T T = T_0; T.ind < TAU; next_T(&T)){
                    if(i_hold_T(j, T))
                        f_copy(c[l][T0.ind * TAU_i + T1.ind][T.ind] , c_i[l][j][T0.ind * TAU_i + T1.ind][T_ind++]);
                }
            }}
        }
    }
    /////////////////////////////////////////////
    free(c);
}


/////////////////////////////////////////////
// Re-indexes the outputs so that
// output of party i is sent to party j 
// Not needed in real life execution
// Just in the simulation where we simulate all parties
/////////////////////////////////////////////
void mal_setup_reindex_outputs(RSS_i c[CONST_N][LAMBDA][CONST_N][TAU_i * TAU_i]){
    // f_elm_t c_i[party i that computes output][LAMBDA][party j that receives from i][TAU_i * TAU_i shares of computer][TAU_j * TAU_j shares of receiver]
    RSS_i (*temp) = malloc(sizeof(RSS_i[TAU_i * TAU_i]));

    for(int i = 0; i < CONST_N; i++){
        for(int l = 0; l < LAMBDA; l++){
            for(int j = 0; j < i; j++){
                memcpy(temp,        c[j][l][i], sizeof(RSS_i[TAU_i * TAU_i]));
                memcpy(c[j][l][i],  c[i][l][j], sizeof(RSS_i[TAU_i * TAU_i]));
                memcpy(c[i][l][j],  temp,       sizeof(RSS_i[TAU_i * TAU_i]));
            }
        }
    }
    free(temp);
}

/////////////////////////////////////////////
// Second phase - reconstructing the product
// Parties receive outputs, verify consistency of shares
// and reconstruct the output
/////////////////////////////////////////////
int mal_setup_mul_2(RSS_i c_i[LAMBDA][CONST_N][TAU_i * TAU_i], RSS_i s2_i[LAMBDA]){
    int test = 0;

    for(int l = 0; l < LAMBDA; l++)
        test |= mult_RSS_to_RSS_reconstruction(c_i[l], s2_i[l]);

    /////////////////////////////////////////////

    return test;
}

/////////////////////////////////////////////
// MALICIOUS EVALUATION
/////////////////////////////////////////////
void mal_evaluation(const RSS_i x_i, const RSS_i k_i[LAMBDA], const RSS_i s2_i[LAMBDA], const DRSS_i r_i[LAMBDA], const ASS_i az_i[LAMBDA][TAU_i * TAU_i][3], DRSS_i o_i[LAMBDA], hash_digest h_i)
{
    RSS_i a_i;
    f_elm_t t0, t1;

    f_elm_t(*temp) = malloc(sizeof(f_elm_t[TAU_i * TAU_i * LAMBDA]));

    for (int j = 0; j < LAMBDA; j++)
    {
        add_RSS_i(x_i, k_i[j], a_i);

        for (share_T T0 = T_0; T0.ind < TAU_i; next_T(&T0))
        {
            for (share_T T1 = T_0; T1.ind < TAU_i; next_T(&T1))
            {

                f_mul(a_i[T0.ind], s2_i[j][T1.ind], t0);        // t0 = a_{T_0} * b_{T_1}
                f_add(t0, r_i[j][T0.ind * TAU_i + T1.ind], t0); // t0 = a_{T_0} * b_{T_1} + r_{T_0T_1} = o_{T_1T_2}

                f_copy(t0, temp[(TAU_i * TAU_i) * j + T0.ind * TAU_i + T1.ind]); // copy and store a_{T_0} * b_{T_1} + r_{T_0T_1}

                f_mul(a_i[T0.ind], az_i[j][T0.ind * TAU_i + T1.ind][0][0], t1); // t1 = a_{T_0} * t^{a, T_0T_1}
                f_add(t0, t1, t0);                                              // t0 = a_{T_0} * b_{T_1} + r_{T_0T_1} + a_{T_0} * t^{a, T_0T_1}_i

                f_mul(s2_i[j][T1.ind], az_i[j][T0.ind * TAU_i + T1.ind][1][0], t1); // t1 = b_{T_1} * t^{b, T_0T_1}_i
                f_add(t0, t1, t0);                                                  // t0 = a_{T_0} * b_{T_1} + r_{T_0T_1} + a_{T_0} * t^{a, T_0T_1}_i + b_{T_1} * t^{b, T_0T_1}_i

                f_add(t0, az_i[j][T0.ind * TAU_i + T1.ind][2][0], t0); // t0 = a_{T_0} * b_{T_1} + r_{T_0T_1} + a_{T_0} * t^{a, T_0T_1}_i + b_{T_1} * t^{b, T_0T_1}_i + t^{r, T_0T_1}_i

                f_copy(t0, o_i[j][T0.ind * TAU_i + T1.ind]);
            }
        }
    }

    hash_array(temp, LAMBDA * TAU_i * TAU_i, h_i);

    free(temp);
}

int main(int argc, char const *argv[])
    {
        int test = 0;
        (void)test;
        int SERVER_ID = 0;

        init_inverses();
        generate_table();

        prng_seed seed = {0};
        prng_seed curr_seed;
        memcpy(curr_seed, seed, NBYTES_SEED);

        ////////////////////////////////////////////
        // Generate key
        // Same key as inside mal_tp_setup
        ////////////////////////////////////////////
        RSS(*rk_temp) = malloc(sizeof(RSS[LAMBDA]));
        f_elm_t(*k) = malloc(sizeof(f_elm_t[LAMBDA]));

        RSS_rand_array(rk_temp, curr_seed);
        for (int j = 0; j < LAMBDA; j++)
            open_RSS(rk_temp[j], k[j]);

        memcpy(curr_seed, seed, NBYTES_SEED);
        free(rk_temp);
        ////////////////////////////////////////////

        struct timespec start, end;
        long elapsed_ns;
        double elapsed_us;

        RSS_i(*k_i)[LAMBDA] = malloc(sizeof(RSS_i[CONST_N][LAMBDA]));
        ////////////////////////////////////////////
        // TRUSTED PARTY SEED AND KEY DISTRIBUTION
        ////////////////////////////////////////////

        RSS_seed_i(*rseed_i) = malloc(sizeof(RSS_seed_i[CONST_N]));
        DRSS_seed_i(*dseed_i) = malloc(sizeof(DRSS_seed_i[CONST_N]));
        ASS_seed_zero_i(*aseedz_i) = malloc(sizeof(ASS_seed_zero_i[CONST_N]));
        RSS_seed_zero_i(*rseedz_i) = malloc(sizeof(RSS_seed_zero_i[CONST_N]));
        DRSS_seed_zero_i(*dseedz_i) = malloc(sizeof(DRSS_seed_zero_i[CONST_N]));

        mal_tp_setup(seed, k_i, rseed_i, dseed_i, aseedz_i, rseedz_i, dseedz_i);
        ////////////////////////////////////////////

        // Get the start time before dealing with messages
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
        {
            perror("clock_gettime");
        }
        ////////////////////////////////////////////
        // LOCAL SETUP STAGE
        ////////////////////////////////////////////
        ASS_i(*az_i)[LAMBDA][TAU_i * TAU_i][3] = malloc(sizeof(ASS_i[CONST_N][LAMBDA][TAU_i * TAU_i][3]));
        RSS_i(*rz_i)[LAMBDA] = malloc(sizeof(RSS_i[CONST_N][LAMBDA]));
        DRSS_i(*dz_i)[LAMBDA] = malloc(sizeof(DRSS_i[CONST_N][LAMBDA]));

        ////////////////////////////////////////////
        // Compute all secret shares of zero to be used later

        for (int i = 0; i < CONST_N; i++)
            mal_setup_zeros(i, aseedz_i[i], rseedz_i[i], dseedz_i[i], az_i[i], rz_i[i], dz_i[i]);

        ////////////////////////////////////////////
        // First step of RSS multiplication

        RSS_i(*c_i)[LAMBDA][CONST_N][TAU_i * TAU_i] = malloc(sizeof(RSS_i[CONST_N][LAMBDA][CONST_N][TAU_i * TAU_i]));

        for (int i = 0; i < CONST_N; i++)
            mal_setup_mul_1(rz_i[i], c_i[i], rseed_i[i], dseed_i[i]);

        ////////////////////////////////////////////
        // Re-indexing, i.e., sending messages

        mal_setup_reindex_outputs(c_i);

        ////////////////////////////////////////////
        // Second step of RSS multiplication

        RSS_i(*s2_i)[LAMBDA] = malloc(sizeof(RSS_i[CONST_N][LAMBDA]));

        for (int i = 0; i < CONST_N; i++)
            test |= mal_setup_mul_2(c_i[i], s2_i[i]);

        ////////////////////////////////////////////
        free(rz_i);
        free(c_i);
        free(rseed_i);
        free(dseed_i);
        free(aseedz_i);
        free(rseedz_i);
        free(dseedz_i);

        // printf("Complete setup phase. \n");

        if (clock_gettime(CLOCK_MONOTONIC, &end) == -1)
        {
            perror("clock_gettime");
        }

        if (argc > 1)
        {
            SERVER_ID = atoi(argv[1]);
        }

        char filename[256];
        snprintf(filename, sizeof(filename), "server_offline_%d_%d_%d_%d.txt", ADVERSARY, SERVER_ID, CONST_T, CONST_N);

        // Calculate the elapsed time in milliseconds
        elapsed_ns = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
        elapsed_us = elapsed_ns / 1e3;

        // printf("Total time required computing preprocessing messages: %.2f µs\n", elapsed_us / CONST_N);

        // Write the elapsed time to the file
        FILE *file = fopen(filename, "w");
        if (file == NULL)
        {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fprintf(file, "%.2f\n", elapsed_us / CONST_N);
        fclose(file);

        /**Network part initialization*/
        int port = PORT + SERVER_ID;

        int server_fd, new_socket;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Forcefully attaching socket to the port
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("bind failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // printf("Server listening on port %d\n", port);
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        /**Network part initialization*/

        uint8_t buffer[sizeof(RSS_i)] = {0};
        size_t read_size = read(new_socket, buffer, sizeof(RSS_i));
        if (read_size > 0)
        {
            // printf("Recieve message from client successfully! \n");
        }

        // Deserialize RSS_i data
        RSS_i *x_i = malloc(sizeof(RSS_i));
    deserialize_RSS_i(buffer, x_i);

    // printf("Server debug 0\n");

    /*********Here starts the server response************/
    ////////////////////////////////////////////
    // SERVERS EVALUATION STAGE
    ////////////////////////////////////////////
    size_t total_response_size=0;
    
    DRSS_i(*o_i) = malloc(LAMBDA * sizeof(DRSS_i));
    hash_digest(*h_i) = malloc(sizeof(hash_digest));
    mal_evaluation(*x_i, k_i[SERVER_ID], s2_i[SERVER_ID], dz_i[SERVER_ID], az_i[SERVER_ID], o_i, *h_i);
    ////////////////////////////////////////////
    free(dz_i);

    total_response_size = sizeof(DRSS_i) * LAMBDA + sizeof(hash_digest);
    uint8_t *response = malloc(total_response_size);
    // Serialize the data into the response buffer
    int offset = 0;
    for (int i = 0; i < LAMBDA; i++)
    {
        serialize_DRSS_i(&(o_i)[i], response + offset);
        offset += sizeof(DRSS_i);
    }
    memcpy(response + offset, h_i, sizeof(hash_digest));
    
    free(h_i);
    // printf("Response sent\n");
    /*********Here ends the server response************/
    send(new_socket, response, total_response_size, 0);
    free(response);
    free(x_i);
    free(k_i);
    free(s2_i);
    free(az_i);
    // free(o_i);

    close(new_socket);
    close(server_fd);

    // struct timespec now;
    // // Get the current time
    // clock_gettime(CLOCK_REALTIME, &now);
    // // Convert timespec to time_t for easier formatting
    // time_t seconds = now.tv_sec;              // Get seconds part
    // struct tm *tm_info = localtime(&seconds); // Convert to tm struct
    // // Print current time
    // char timeBuffer[26];
    // strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", tm_info);
    // printf("server finish its task by timestamp %s.%09ld\n", timeBuffer, now.tv_nsec);

    return 0;
}
