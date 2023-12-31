class Saes(object):
    # class SAES has all functions used and has predefined poperties rk0 rk1 and rk2 generated by passing a key value to class there is no default
    # S-Box used ----
    sbox = [0x9,0x4,0xA,0xB,0xD,0x1,0x8,0x5,0x6,0x2,0x0,0x3,0xC,0xE,0xF,0x7,]
    # Inverse S-Box used -----
    sboxI = [0xA,0x5,0x9,0xB,0x1,0x7,0x8,0xF,0x6,0x0, 0x2,0x3,0xC,0x4,  0xD, 0xE,]

    def __init__(self, key):
        # Round keys are  K0 = w0 + w1; K1 = w2 + w3; K2 = w4 + w5
        self.rk0, self.rk1, self.rk2 = self.key_expansion(key)

    def key_expansion(self, key):
        # Round constants
        rcon1 = 0x80
        rcon2 = 0x30

        # Calculating value of each word
        w = [None] * 6
        #empty initially
        w[0] = (key & 0xFF00) >> 8
        #first 8 bits
        #w0 as key in masked ie hex in 4
        w[1] = key & 0x00FF
        # next 8 bits of rk0
        #the rst of the key as w1
        w[2] = w[0] ^ (self.nibble_replacement(self.rotate_word_keygen(w[1])) ^ rcon1)
        #generated by rotate w1 and xoring with rcon1 and then xoring with w0
        w[3] = w[2] ^ w[1]
        #self explainatory
        w[4] = w[2] ^ (self.nibble_replacement(self.rotate_word_keygen(w[3])) ^ rcon2)
        #similarto two steps above
        w[5] = w[4] ^ w[3]
        # rk0 is w0 and w1
        # rk1 is w2 and w3
        # rk2 is w4 and w5

        return (
            self.int_to_state((w[0] << 8) + w[1]),  # Pre-Round key
            self.int_to_state((w[2] << 8) + w[3]),  # Round 1 key
            self.int_to_state((w[4] << 8) + w[5]),  # Round 2 key
        )

    def rotate_word_keygen(self, word):
        # Swapping the two nibbles in the word since eqv to rotate here
        #left right parts swap
        #masks and then left shift
        return ((word & 0x0F) << 4) + ((word & 0xF0) >> 4)

    def nibble_replacement(self, word):
        # Take each nibble in the word and substitute another nibble for it using the sbox table
        #################
        return (self.sbox[(word >> 4)] << 4) + self.sbox[word & 0x0F]

    def field_multiplication(self, a, b):
        mult = 0
        #masking is done here
        a = a & 0x0F
        b = b & 0x0F
        #a and b should be non zero
        while a and b:
            # lsb is one
            if b & 1:
                #update mult
                mult = mult ^ a
            #update a to shift
            a = a << 1
            #if goes beyond 4th bit
            if a & (1 << 4):
                # XOR with irreducible polynomial with high term eliminated
                a = a ^ 0b10011
            # Update b to b // 2
            b = b >> 1
        return mult

    def add_round_key(self, s1, s2):
        #zoring for every digit in key state pair zip is used to create pairs
        return [i ^ j for i, j in zip(s1, s2)]

    def shift_rows(self, state):
        """from s00 , s01, s10 , s11 to s00, s01 , s11, s10 where s is state and sxx are words at index
        """
        return [state[0], state[1], state[3], state[2]]

    def mix_columns(self, state):
        """Mix columns transformation on state matrix
        """
        return [
            state[0] ^ self.field_multiplication(4, state[2]),
            state[1] ^ self.field_multiplication(4, state[3]),
            state[2] ^ self.field_multiplication(4, state[0]),
            state[3] ^ self.field_multiplication(4, state[1]),
            ]

    def state_to_int(self, m):
        """similar to int to state but reverse
        """
        return (m[0] << 12) + (m[2] << 8) + (m[1] << 4) + m[3]

    def int_to_state(self, n):
        """converts binary 16 bit into hexadecimal to save in array s00, s01,s10,s11 by right shifting by four each time and taking converting into hex
        """
        return [n >> 12 & 0xF, (n >> 4) & 0xF, (n >> 8) & 0xF, n & 0xF]

    def inverse_mix_columns(self, state):
        """similar to above in inverse
        """
        return [
            self.field_multiplication(9, state[0]) ^ self.field_multiplication(2, state[2]),
            self.field_multiplication(9, state[1]) ^ self.field_multiplication(2, state[3]),
            self.field_multiplication(9, state[2]) ^ self.field_multiplication(2, state[0]),
            self.field_multiplication(9, state[3]) ^ self.field_multiplication(2, state[1]),
            ]

    def ns(self, sbox, state):
        """Nibble substitution
        finds sub in sbox array at index state
        """
        return [sbox[nibble] for nibble in state]

    def decrypt(self, ciphertext):
        state = self.add_round_key(self.rk2, self.int_to_state(ciphertext))
        state = self.ns(self.sboxI, self.shift_rows(state))
        state = self.inverse_mix_columns(self.add_round_key(self.rk1, state))
        state = self.ns(self.sboxI, self.shift_rows(state))
        state = self.add_round_key(self.rk0, state)
        return self.state_to_int(state)
    def encrypt(self, plaintext):
        """Encrypt plaintext with given key
        Example::
            ciphertext = Saes(key=0b0100101011110101).encrypt(0b1101011100101000)
        :param plaintext: 16 bit plaintext
        :returns: 16 bit ciphertext
        """
        state = self.add_round_key(self.rk0, self.int_to_state(plaintext))

        state = self.mix_columns(self.shift_rows(self.ns(self.sbox, state)))

        state = self.add_round_key(self.rk1, state)

        state = self.shift_rows(self.ns(self.sbox, state))

        state = self.add_round_key(self.rk2, state)

        return self.state_to_int(state)
# op= int(input("press 1 for encrypt , 2 to decrypt:"))
# if op ==1:
#     plaintext=int(input("enter plaintext"))
#     pt = "{0:b}".format(int(plaintext))
#     key = int(input("enter key"))
#     k = "{0:b}".format(int(key))
# ciphertext = Saes(key=key).encrypt(pt)
ciphertext= Saes(key=0b1010011100111011).encrypt(0b0110111101101011)
print("need to enter key and text manually due to format restrictions")
print("enc is")
# temp = int(input("get temp"))
# print(type(temp))
print("{0:b}".format(int(ciphertext)))

dec = Saes(key=0b0100101011110101).decrypt(ciphertext)

print("dec is ")
print("{0:b}".format(int(dec)))


# key = 0100101011110101
#  plain= 1101011100101000