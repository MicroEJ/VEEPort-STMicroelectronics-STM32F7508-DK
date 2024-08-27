/*
 * C
 *
 * Copyright 2015-2023 MicroEJ Corp. All rights reserved.
 * This library is provided in source code for use, modification and test, subject to license terms.
 * Any modification of the source code will break MicroEJ Corp. warranties on the whole library.
 */

#ifndef LLSEC_ERRORS_H
#define LLSEC_ERRORS_H
		
/**
 * @file
 * @brief MicroEJ Security low level API
 * @author MicroEJ Developer Team
 * @version 2.4.0
 * @date 16 February 2024
 */

#define J_SEC_NO_ERROR							 0 /* not an error */
#define J_EOF									-1 /* end of file */
#define J_NATIVE_CODE_BLOCKED_WITHOUT_RESULT	-4 /* the native code has been stopped (with a timeout or not) because result was not available */
#define J_BLOCKING_QUEUE_LIMIT_REACHED			-5 /* Dispatch blocking request queue is full and a new request can not be added now */
#define J_CERT_PARSE_ERROR						-6 /* certificate parsing error */
#define J_BIG_CERT_OR_KEY_SIZE_ERROR			-7; /* certificate or key size bigger than the immortal buffer used to process certificates and keys */
#define J_NO_TRUSTED_CERT						-8 /* No trusted certificate found*/
#define J_NOT_CA_CERT							-9 /* basic constraints check failed: intermediate certificate is not a CA certificate */
#define J_NAME_CHAINING_ERROR					-10 /* subject/issuer name chaining error*/
#define J_X509_CERT_EXPIRED_ERROR				-11 /* X509 certificate checkValidity error: expired*/
#define J_X509_CERT_NOT_YET_VALID_ERROR			-12 /* X509 certificate checkValidity error: not yet valid*/

#define J_RSA_WRONG_TYPE_ERROR                 -21 /* RSA wrong block type for RSA function */
#define J_RSA_BUFFER_ERROR                     -22 /* RSA buffer error output too small or input too large */
#define J_BUFFER_ERROR                         -23 /* output buffer too small or input too large */
#define J_ALGO_ID_ERROR                        -24 /* setting algo id error */
#define J_PUBLIC_KEY_ERROR                     -25 /* setting public key error */
#define J_DATE_ERROR                           -26 /* setting date validity error */
#define J_SUBJECT_ERROR                        -27 /* setting subject name error */
#define J_ISSUER_ERROR                         -28 /* setting issuer  name error */
#define J_CA_TRUE_ERROR                        -29 /* setting CA basic constraint true error */
#define J_EXTENSIONS_ERROR                     -30 /* setting extensions error */

#define J_ASN_VERSION_ERROR                    -31 /* ASN version error invalid number */
#define J_ASN_GETINT_ERROR                     -32 /* ASN get big int error invalid data */
#define J_ASN_RSA_KEY_ERROR                    -33 /* ASN key init error invalid input */
#define J_ASN_OBJECT_ID_ERROR                  -34 /* ASN object id error invalid id */
#define J_ASN_TAG_NULL_ERROR                   -35 /* ASN tag error not null */
#define J_ASN_EXPECT_0_ERROR                   -36 /* ASN expect error not zero */
#define J_ASN_BITSTR_ERROR                     -37 /* ASN bit string error wrong id */
#define J_ASN_UNKNOWN_OID_ERROR                -38 /* ASN oid error unknown sum id */
#define J_ASN_DATE_SZ_ERROR                    -39 /* ASN date error bad size */
#define J_ASN_BEFORE_DATE_ERROR                -40 /* ASN date error current date before */
#define J_ASN_AFTER_DATE_ERROR                 -41 /* ASN date error current date after */
#define J_ASN_SIG_OID_ERROR                    -42 /* ASN signature error mismatched oid */
#define J_ASN_TIME_ERROR                       -43 /* ASN time error unknown time type */
#define J_ASN_INPUT_ERROR                      -44 /* ASN input error not enough data */
#define J_ASN_SIG_CONFIRM_ERROR                -45 /* ASN signature error confirm failure */
#define J_ASN_SIG_HASH_ERROR                   -46 /* ASN signature error unsupported hash type */
#define J_ASN_SIG_KEY_ERROR                    -47 /* ASN signature error unsupported key type */
#define J_ASN_DH_KEY_ERROR                     -48 /* ASN key init error invalid input */
#define J_ASN_NTRU_KEY_ERROR                   -49 /* ASN ntru key decode error invalid input */
#define J_ASN_CRIT_EXT_ERROR                   -50 /* ASN unsupported critical extension */
#define J_ASN_NO_SIGNER_ERROR     		        -51 /* ASN no signer to confirm failure */
#define J_ASN_CRL_CONFIRM_ERROR   		        -52 /* ASN CRL signature confirm failure */
#define J_ASN_CRL_NO_SIGNER_ERROR 		        -53 /* ASN CRL no signer to confirm failure */
#define J_ASN_OCSP_CONFIRM_ERROR  		        -54 /* ASN OCSP signature confirm failure */

#define J_ECC_BAD_ARG_ERROR                   -60 /* ECC input argument of wrong type */
#define J_ASN_ECC_KEY_ERROR                   -61 /* ASN ECC bad input */
#define J_ECC_CURVE_OID_ERROR                 -62 /* Unsupported ECC OID curve type */
#define J_BAD_FUNC_ARG                        -63 /* Bad function argument provided */
#define J_NOT_COMPILED_IN                     -64 /* Feature not compiled in */
#define J_UNICODE_SIZE_ERROR                  -65 /* Unicode password too big */
#define J_NO_PASSWORD         		           -66 /* no password provided by user */
#define J_ALT_NAME_ERROR         		       -67 /* alt name size problem too big */

#define J_AES_GCM_AUTH_ERROR     		       -70 /* AES-GCM Authentication check failure */
#define J_AES_CCM_AUTH_ERROR     		       -71 /* AES-CCM Authentication check failure */

#define J_CAVIUM_INIT_ERROR      		       -80 /* Cavium Init type error */
#define J_BAD_ALIGN_ERROR         		       -81 /* Bad alignment for operation no alloc */
#define J_BAD_ENC_STATE_ERROR     		       -82 /* Bad ecc enc state operation */
#define J_BAD_PADDING_ERROR       		       -83 /* Bad padding msg not correct length  */
#define J_REQ_ATTRIBUTE_ERROR     		       -84 /* setting cert request attributes error */
#define J_PKCS7_OID_ERROR         		       -85 /* PKCS#7 mismatched OID error */
#define J_PKCS7_RECIP_ERROR       		       -86 /* PKCS#7 recipient error */
#define J_FIPS_NOT_ALLOWED_ERROR  		       -87 /* FIPS not allowed error */
#define J_ASN_NAME_INVALID_ERROR  		       -88 /* ASN name constraint error */
#define J_RNG_FAILURE_ERROR       		       -89 /* RNG Failed Reinitialize */
#define J_HMAC_MIN_KEYLEN_ERROR   		       -90 /* FIPS Mode HMAC Minimum Key Length error */
#define J_RSA_PAD_ERROR           		       -91 /* RSA Padding Error */
#define J_LENGTH_ONLY_ERROR       		       -92 /* Returning output length only */
#define J_IN_CORE_FIPS_ERROR      		       -93 /* In Core Integrity check failure */
#define J_AES_KAT_FIPS_ERROR      		       -94 /* AES KAT failure */
#define J_DES3_KAT_FIPS_ERROR     		       -95 /* DES3 KAT failure */
#define J_HMAC_KAT_FIPS_ERROR     		       -96 /* HMAC KAT failure */
#define J_RSA_KAT_FIPS_ERROR      		       -97 /* RSA KAT failure */
#define J_DRBG_KAT_FIPS_ERROR     		       -98 /* HASH DRBG KAT failure */
#define J_DRBG_CONT_FIPS_ERROR    		       -99 /* HASH DRBG Continious test failure */
#define J_AESGCM_KAT_FIPS_ERROR   		       -100 /* AESGCM KAT failure */
#define J_INPUT_CASE_ERROR                    -101 /* process input state error */
#define J_PREFIX_ERROR                        -102 /* bad index to key rounds  */
#define J_MEMORY_ERROR                        -103 /* out of memory            */
#define J_ENCRYPT_ERROR                       -114 /* error during encryption  */
#define J_NO_PEER_KEY                         -116 /* need peer's key          */
#define J_NO_PRIVATE_KEY                      -117 /* need the private key     */
#define J_RSA_PRIVATE_ERROR                   -118 /* error during rsa priv op */
#define J_NO_DH_PARAMS                        -119 /* server missing DH params */
#define J_BUILD_MSG_ERROR                     -120 /* build message failure    */
#define J_BAD_HELLO                           -121 /* client hello malformed   */
#define J_DOMAIN_NAME_MISMATCH                -122 /* peer subject name mismatch */
#define J_WANT_READ                           -123 /* want read call again    */
#define J_NOT_READY_ERROR                     -124 /* handshake layer not ready */
#define J_PMS_VERSION_ERROR                   -125 /* pre m secret version error */
#define J_VERSION_ERROR                       -126 /* record layer version error */
#define J_WANT_WRITE                          -127 /* want write call again   */
#define J_MALFORMED_BUFFER                    -128 /* malformed buffer input   */
#define J_VERIFY_CERT_ERROR                   -129 /* verify cert error        */
#define J_VERIFY_SIGN_ERROR                   -130 /* verify sign error        */
#define J_CLIENT_ID_ERROR                     -131 /* psk client identity error  */
#define J_SERVER_HINT_ERROR                   -132 /* psk server hint error  */
#define J_PSK_KEY_ERROR                       -133 /* psk key error  */
#define J_LENGTH_ERROR                        -134 /* record layer length error */
#define J_PEER_KEY_ERROR                      -135 /* can't decode peer key */
#define J_ZERO_RETURN                         -136 /* peer sent close notify */
#define J_SIDE_ERROR                          -137 /* wrong client/server type */
#define J_NO_PEER_CERT                        -138 /* peer didn't send key */

#define J_NTRU_KEY_ERROR                      -140 /* NTRU key error  */
#define J_NTRU_DRBG_ERROR                     -141 /* NTRU drbg error  */
#define J_NTRU_ENCRYPT_ERROR                  -142 /* NTRU encrypt error  */
#define J_NTRU_DECRYPT_ERROR                  -143 /* NTRU decrypt error  */

#define J_ECC_CURVETYPE_ERROR                 -150 /* Bad ECC Curve Type */
#define J_ECC_CURVE_ERROR                     -151 /* Bad ECC Curve */
#define J_ECC_PEERKEY_ERROR                   -152 /* Bad Peer ECC Key */
#define J_ECC_MAKEKEY_ERROR                   -153 /* Bad Make ECC Key */
#define J_ECC_EXPORT_ERROR                    -154 /* Bad ECC Export Key */
#define J_ECC_SHARED_ERROR                    -155 /* Bad ECC Shared Secret */
#define J_NOT_CA_ERROR                        -157 /* Not a CA cert error */
#define J_BAD_CERT_MANAGER_ERROR              -159 /* Bad Cert Manager */
#define J_OCSP_CERT_REVOKED                   -160 /* OCSP Certificate revoked */
#define J_CRL_CERT_REVOKED                    -161 /* CRL Certificate revoked */
#define J_CRL_MISSING                         -162 /* CRL Not loaded */
#define J_OCSP_NEED_URL                       -165 /* OCSP need an URL for lookup */
#define J_OCSP_CERT_UNKNOWN                   -166 /* OCSP responder doesn't know */
#define J_OCSP_LOOKUP_FAIL                    -167 /* OCSP lookup not successful */
#define J_MAX_CHAIN_ERROR                     -168 /* max chain depth exceeded */
#define J_SUITES_ERROR                        -171 /* suites pointer error */
#define J_SEC_NO_PEM_HEADER                   -172 /* no PEM header found */
#define J_OUT_OF_ORDER_ERROR                  -173 /* out of order message */
#define J_BAD_KEA_TYPE_ERROR                  -174 /* bad KEA type found */
#define J_SANITY_CIPHER_ERROR                 -175 /* sanity check on cipher error */
#define J_RECV_OVERFLOW_ERROR                 -176 /* RXCB returned more than rqed */
#define J_NO_PEER_VERIFY                      -178 /* Need peer cert verify Error */
#define J_UNKNOWN_HOST_NAME_ERROR             -181 /* Unrecognized host name Error */
#define J_UNKNOWN_MAX_FRAG_LEN_ERROR          -182 /* Unrecognized max frag len Error */
#define J_KEYUSE_SIGNATURE_ERROR              -183 /* KeyUse digSignature error */
#define J_KEYUSE_ENCIPHER_ERROR               -185 /* KeyUse keyEncipher error */
#define J_EXTKEYUSE_AUTH_ERROR                -186 /* ExtKeyUse server|client_auth */
#define J_SEND_OOB_READ_ERROR                 -187 /* Send Cb out of bounds read */
#define J_SECURE_RENEGOTIATION_ERROR          -188 /* Invalid Renegotiation Info */
#define J_SCR_DIFFERENT_CERT_ERROR            -189 /* SCR Different cert error  */
#define J_NO_CHANGE_CIPHER_ERROR              -190 /* Finished before change cipher */
#define J_SANITY_MSG_ERROR                    -191 /* Sanity check on msg order error */
#define J_DUPLICATE_MSG_ERROR                 -192 /* Duplicate message error */
#define J_UNSUPPORTED_SUITE                   -193 /* unsupported cipher suite */
#define J_MATCH_SUITE_ERROR                   -194 /* can't match cipher suite */
#define J_BAD_CERTTYPE     				   	  -195 /* bad certificate type */
#define J_BAD_ENCODED_CERT_FORMAT     		  -196 /* bad file type */
#define J_OPEN_RAN_ERROR		  			  -197 /* opening random device error */
#define J_READ_RAN_ERROR         		      -198 /* reading random device error */
#define J_WINCRYPT_ERROR         		      -199 /* windows crypt init error */
#define J_CRYPTGEN_ERROR         		      -200 /* windows crypt generation error */
#define J_RAN_BLOCK_ERROR        		      -201 /* reading random device would block */
#define J_UNKNOWN_ERROR					   	  -202 /* unknown error */

#endif //LLSEC_ERRORS_H
