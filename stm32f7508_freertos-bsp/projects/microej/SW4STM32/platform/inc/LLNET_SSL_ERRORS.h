/*
 * C
 *
 * Copyright 2015-2022 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef __LLNET_SSL_ERRORS__
#define __LLNET_SSL_ERRORS__
		
/**
 * @file
 * @brief MicroEJ SSL low level API
 * @author MicroEJ Developer Team
 * @version 6.0.0
 * @date 26 August 2022
 */

#define J_SSL_NO_ERROR							 0 /* @brief not an error */
#define J_EOF									-1 /* @brief end of file */
#define J_CONNECTION_RESET						-2 /* @brief connection reset by the peer */
#define J_SOCKET_TIMEOUT						-3 /* @brief socket timeout */
#define J_BLOCKING_QUEUE_LIMIT_REACHED			-5 /* @brief Dispatch blocking request queue is full and a new request can not be added now */
#define J_CERT_PARSE_ERROR						-6 /* @brief certificate parsing error */
#define J_BIG_CERT_OR_KEY_SIZE_ERROR			-7; /* @brief certificate or key size bigger than the immortal buffer used to process certificates and keys */
#define J_NO_TRUSTED_CERT						-8 /* @brief No trusted certificate found*/
#define J_NOT_CA_CERT							-9 /* @brief basic constraints check failed: intermediate certificate is not a CA certificate */
#define J_NAME_CHAINING_ERROR					-10 /* @brief subject/issuer name chaining error*/
#define J_BIG_KEY_PASSWORD_SIZE_ERROR			-11 /* @brief key password size bigger than the immortal buffer reserved for the keys passwords*/

#define J_RSA_WRONG_TYPE_ERROR                 -21 /* @brief RSA wrong block type for RSA function */
#define J_RSA_BUFFER_ERROR                     -22 /* @brief RSA buffer error output too small or input too large */
#define J_BUFFER_ERROR                         -23 /* @brief output buffer too small or input too large */
#define J_ALGO_ID_ERROR                        -24 /* @brief setting algo id error */
#define J_PUBLIC_KEY_ERROR                     -25 /* @brief setting public key error */
#define J_DATE_ERROR                           -26 /* @brief setting date validity error */
#define J_SUBJECT_ERROR                        -27 /* @brief setting subject name error */
#define J_ISSUER_ERROR                         -28 /* @brief setting issuer  name error */
#define J_CA_TRUE_ERROR                        -29 /* @brief setting CA basic constraint true error */
#define J_EXTENSIONS_ERROR                     -30 /* @brief setting extensions error */

#define J_ASN_VERSION_ERROR                    -31 /* @brief ASN version error invalid number */
#define J_ASN_GETINT_ERROR                     -32 /* @brief ASN get big int error invalid data */
#define J_ASN_RSA_KEY_ERROR                    -33 /* @brief ASN key init error invalid input */
#define J_ASN_OBJECT_ID_ERROR                  -34 /* @brief ASN object id error invalid id */
#define J_ASN_TAG_NULL_ERROR                   -35 /* @brief ASN tag error not null */
#define J_ASN_EXPECT_0_ERROR                   -36 /* @brief ASN expect error not zero */
#define J_ASN_BITSTR_ERROR                     -37 /* @brief ASN bit string error wrong id */
#define J_ASN_UNKNOWN_OID_ERROR                -38 /* @brief ASN oid error unknown sum id */
#define J_ASN_DATE_SZ_ERROR                    -39 /* @brief ASN date error bad size */
#define J_ASN_BEFORE_DATE_ERROR                -40 /* @brief ASN date error current date before */
#define J_ASN_AFTER_DATE_ERROR                 -41 /* @brief ASN date error current date after */
#define J_ASN_SIG_OID_ERROR                    -42 /* @brief ASN signature error mismatched oid */
#define J_ASN_TIME_ERROR                       -43 /* @brief ASN time error unknown time type */
#define J_ASN_INPUT_ERROR                      -44 /* @brief ASN input error not enough data */
#define J_ASN_SIG_CONFIRM_ERROR                -45 /* @brief ASN signature error confirm failure */
#define J_ASN_SIG_HASH_ERROR                   -46 /* @brief ASN signature error unsupported hash type */
#define J_ASN_SIG_KEY_ERROR                    -47 /* @brief ASN signature error unsupported key type */
#define J_ASN_DH_KEY_ERROR                     -48 /* @brief ASN key init error invalid input */
#define J_ASN_NTRU_KEY_ERROR                   -49 /* @brief ASN ntru key decode error invalid input */
#define J_ASN_CRIT_EXT_ERROR                   -50 /* @brief ASN unsupported critical extension */
#define J_ASN_NO_SIGNER_ERROR     		        -51 /* @brief ASN no signer to confirm failure */
#define J_ASN_CRL_CONFIRM_ERROR   		        -52 /* @brief ASN CRL signature confirm failure */
#define J_ASN_CRL_NO_SIGNER_ERROR 		        -53 /* @brief ASN CRL no signer to confirm failure */
#define J_ASN_OCSP_CONFIRM_ERROR  		        -54 /* @brief ASN OCSP signature confirm failure */

#define J_ECC_BAD_ARG_ERROR                   -60 /* @brief ECC input argument of wrong type */
#define J_ASN_ECC_KEY_ERROR                   -61 /* @brief ASN ECC bad input */
#define J_ECC_CURVE_OID_ERROR                 -62 /* @brief Unsupported ECC OID curve type */
#define J_BAD_FUNC_ARG                        -63 /* @brief Bad function argument provided */
#define J_NOT_COMPILED_IN                     -64 /* @brief Feature not compiled in */
#define J_UNICODE_SIZE_ERROR                  -65 /* @brief Unicode password too big */
#define J_NO_PASSWORD         		           -66 /* @brief no password provided by user */
#define J_ALT_NAME_ERROR         		       -67 /* @brief alt name size problem too big */

#define J_AES_GCM_AUTH_ERROR     		       -70 /* @brief AES-GCM Authentication check failure */
#define J_AES_CCM_AUTH_ERROR     		       -71 /* @brief AES-CCM Authentication check failure */

#define J_ASYNC_INIT_ERROR      		       -80 /* @brief Async Init error */
#define J_BAD_ALIGN_ERROR         		       -81 /* @brief Bad alignment for operation no alloc */
#define J_BAD_STATE_ERROR     		       	   -82 /* @brief Bad state operation */
#define J_BAD_PADDING_ERROR       		       -83 /* @brief Bad padding msg not correct length  */
#define J_REQ_ATTRIBUTE_ERROR     		       -84 /* @brief setting cert request attributes error */
#define J_PKCS7_OID_ERROR         		       -85 /* @brief PKCS#7 mismatched OID error */
#define J_PKCS7_RECIP_ERROR       		       -86 /* @brief PKCS#7 recipient error */
#define J_FIPS_NOT_ALLOWED_ERROR  		       -87 /* @brief FIPS not allowed error */
#define J_ASN_NAME_INVALID_ERROR  		       -88 /* @brief ASN name constraint error */
#define J_RNG_FAILURE_ERROR       		       -89 /* @brief RNG Failed Reinitialize */
#define J_HMAC_MIN_KEYLEN_ERROR   		       -90 /* @brief FIPS Mode HMAC Minimum Key Length error */
#define J_RSA_PAD_ERROR           		       -91 /* @brief RSA Padding Error */
#define J_LENGTH_ONLY_ERROR       		       -92 /* @brief Returning output length only */
#define J_IN_CORE_FIPS_ERROR      		       -93 /* @brief In Core Integrity check failure */
#define J_AES_KAT_FIPS_ERROR      		       -94 /* @brief AES KAT failure */
#define J_DES3_KAT_FIPS_ERROR     		       -95 /* @brief DES3 KAT failure */
#define J_HMAC_KAT_FIPS_ERROR     		       -96 /* @brief HMAC KAT failure */
#define J_RSA_KAT_FIPS_ERROR      		       -97 /* @brief RSA KAT failure */
#define J_DRBG_KAT_FIPS_ERROR     		       -98 /* @brief HASH DRBG KAT failure */
#define J_DRBG_CONT_FIPS_ERROR    		       -99 /* @brief HASH DRBG Continious test failure */
#define J_AESGCM_KAT_FIPS_ERROR   		       -100 /* @brief AESGCM KAT failure */
#define J_INPUT_CASE_ERROR                    -101 /* @brief process input state error */
#define J_PREFIX_ERROR                        -102 /* @brief bad index to key rounds  */
#define J_MEMORY_ERROR                        -103 /* @brief out of memory            */
#define J_VERIFY_FINISHED_ERROR               -104 /* @brief verify problem on finished */
#define J_VERIFY_MAC_ERROR                    -105 /* @brief verify mac problem       */
#define J_HEADER_PARSE_ERROR                  -106 /* @brief parse error on header    */
#define J_UNKNOWN_HANDSHAKE_TYPE              -107 /* @brief weird handshake type     */
#define J_SOCKET_ERROR                        -108 /* @brief error state on socket    */
#define J_SOCKET_NODATA                       -109 /* @brief expected data not there */
#define J_INCOMPLETE_DATA                     -110 /* @brief don't have enough data to complete task */
#define J_UNKNOWN_RECORD_TYPE                 -111 /* @brief unknown type in record hdr */
#define J_DECRYPT_ERROR                       -112 /* @brief error during decryption  */
#define J_FATAL_ERROR                         -113 /* @brief recvd alert fatal error  */
#define J_ENCRYPT_ERROR                       -114 /* @brief error during encryption  */
#define J_NO_PEER_KEY                         -116 /* @brief need peer's key          */
#define J_NO_PRIVATE_KEY                      -117 /* @brief need the private key     */
#define J_RSA_PRIVATE_ERROR                   -118 /* @brief error during rsa priv op */
#define J_NO_DH_PARAMS                        -119 /* @brief server missing DH params */
#define J_BUILD_MSG_ERROR                     -120 /* @brief build message failure    */
#define J_BAD_HELLO                           -121 /* @brief client hello malformed   */
#define J_DOMAIN_NAME_MISMATCH                -122 /* @brief peer subject name mismatch */
#define J_WANT_READ                           -123 /* @brief want read call again    */
#define J_NOT_READY_ERROR                     -124 /* @brief handshake layer not ready */
#define J_PMS_VERSION_ERROR                   -125 /* @brief pre m secret version error */
#define J_VERSION_ERROR                       -126 /* @brief record layer version error */
#define J_WANT_WRITE                          -127 /* @brief want write call again   */
#define J_MALFORMED_BUFFER                    -128 /* @brief malformed buffer input   */
#define J_VERIFY_CERT_ERROR                   -129 /* @brief verify cert error        */
#define J_VERIFY_SIGN_ERROR                   -130 /* @brief verify sign error        */
#define J_CLIENT_ID_ERROR                     -131 /* @brief psk client identity error  */
#define J_SERVER_HINT_ERROR                   -132 /* @brief psk server hint error  */
#define J_PSK_KEY_ERROR                       -133 /* @brief psk key error  */
#define J_LENGTH_ERROR                        -134 /* @brief record layer length error */
#define J_PEER_KEY_ERROR                      -135 /* @brief can't decode peer key */
#define J_ZERO_RETURN                         -136 /* @brief peer sent close notify */
#define J_SIDE_ERROR                          -137 /* @brief wrong client/server type */
#define J_NO_PEER_CERT                        -138 /* @brief peer didn't send key */

#define J_NTRU_KEY_ERROR                      -140 /* @brief NTRU key error  */
#define J_NTRU_DRBG_ERROR                     -141 /* @brief NTRU drbg error  */
#define J_NTRU_ENCRYPT_ERROR                  -142 /* @brief NTRU encrypt error  */
#define J_NTRU_DECRYPT_ERROR                  -143 /* @brief NTRU decrypt error  */

#define J_ECC_CURVETYPE_ERROR                 -150 /* @brief Bad ECC Curve Type */
#define J_ECC_CURVE_ERROR                     -151 /* @brief Bad ECC Curve */
#define J_ECC_PEERKEY_ERROR                   -152 /* @brief Bad Peer ECC Key */
#define J_ECC_MAKEKEY_ERROR                   -153 /* @brief Bad Make ECC Key */
#define J_ECC_EXPORT_ERROR                    -154 /* @brief Bad ECC Export Key */
#define J_ECC_SHARED_ERROR                    -155 /* @brief Bad ECC Shared Secret */
#define J_NOT_CA_ERROR                        -157 /* @brief Not a CA cert error */
#define J_BAD_CERT_MANAGER_ERROR              -159 /* @brief Bad Cert Manager */
#define J_OCSP_CERT_REVOKED                   -160 /* @brief OCSP Certificate revoked */
#define J_CRL_CERT_REVOKED                    -161 /* @brief CRL Certificate revoked */
#define J_CRL_MISSING                         -162 /* @brief CRL Not loaded */
#define J_OCSP_NEED_URL                       -165 /* @brief OCSP need an URL for lookup */
#define J_OCSP_CERT_UNKNOWN                   -166 /* @brief OCSP responder doesn't know */
#define J_OCSP_LOOKUP_FAIL                    -167 /* @brief OCSP lookup not successful */
#define J_MAX_CHAIN_ERROR                     -168 /* @brief max chain depth exceeded */
#define J_SUITES_ERROR                        -171 /* @brief suites pointer error */
#define J_SSL_NO_PEM_HEADER                   -172 /* @brief no PEM header found */
#define J_OUT_OF_ORDER_ERROR                  -173 /* @brief out of order message */
#define J_BAD_KEA_TYPE_ERROR                  -174 /* @brief bad KEA type found */
#define J_SANITY_CIPHER_ERROR                 -175 /* @brief sanity check on cipher error */
#define J_RECV_OVERFLOW_ERROR                 -176 /* @brief RXCB returned more than rqed */
#define J_NO_PEER_VERIFY                      -178 /* @brief Need peer cert verify Error */
#define J_UNKNOWN_HOST_NAME_ERROR             -181 /* @brief Unrecognized host name Error */
#define J_UNKNOWN_MAX_FRAG_LEN_ERROR          -182 /* @brief Unrecognized max frag len Error */
#define J_KEYUSE_SIGNATURE_ERROR              -183 /* @brief KeyUse digSignature error */
#define J_KEYUSE_ENCIPHER_ERROR               -185 /* @brief KeyUse keyEncipher error */
#define J_EXTKEYUSE_AUTH_ERROR                -186 /* @brief ExtKeyUse server|client_auth */
#define J_SEND_OOB_READ_ERROR                 -187 /* @brief Send Cb out of bounds read */
#define J_SECURE_RENEGOTIATION_ERROR          -188 /* @brief Invalid Renegotiation Info */
#define J_SCR_DIFFERENT_CERT_ERROR            -189 /* @brief SCR Different cert error  */
#define J_NO_CHANGE_CIPHER_ERROR              -190 /* @brief Finished before change cipher */
#define J_SANITY_MSG_ERROR                    -191 /* @brief Sanity check on msg order error */
#define J_DUPLICATE_MSG_ERROR                 -192 /* @brief Duplicate message error */
#define J_UNSUPPORTED_SUITE                   -193 /* @brief unsupported cipher suite */
#define J_MATCH_SUITE_ERROR                   -194 /* @brief can't match cipher suite */
#define J_BAD_CERTTYPE     				   	  -195 /* @brief bad certificate type */
#define J_BAD_ENCODED_CERT_FORMAT     		  -196 /* @brief bad file type */
#define J_OPEN_RAN_ERROR		  			  -197 /* @brief opening random device error */
#define J_READ_RAN_ERROR         		      -198 /* @brief reading random device error */
#define J_WINCRYPT_ERROR         		      -199 /* @brief windows crypt init error */
#define J_CRYPTGEN_ERROR         		      -200 /* @brief windows crypt generation error */
#define J_RAN_BLOCK_ERROR        		      -201 /* @brief reading random device would block */
#define J_UNKNOWN_ERROR					   	  -202 /* @brief unknown error */

#endif //__LLNET_SSL_ERRORS__
