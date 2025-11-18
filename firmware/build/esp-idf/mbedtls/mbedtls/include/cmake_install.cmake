# Install script for directory: /mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/sergio/.espressif/tools/xtensa-esp-elf/esp-14.2.0_20241119/xtensa-esp-elf/bin/xtensa-esp32-elf-objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aes.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/aria.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/asn1write.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/base64.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/bignum.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/block_cipher.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/build_info.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/camellia.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ccm.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chacha20.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/chachapoly.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/check_config.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cipher.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/cmac.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/compat-2.x.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_crypto.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_legacy_from_psa.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_from_legacy.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_psa_superset_legacy.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_ssl.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_adjust_x509.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/config_psa.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/constant_time.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ctr_drbg.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/debug.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/des.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/dhm.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdh.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecdsa.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecjpake.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ecp.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/entropy.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/error.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/gcm.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hkdf.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/hmac_drbg.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/lms.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/mbedtls_config.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/md5.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/net_sockets.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/nist_kw.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/oid.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pem.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pk.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs12.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs5.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/pkcs7.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_time.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/platform_util.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/poly1305.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/private_access.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/psa_util.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ripemd160.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/rsa.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha1.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha256.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha3.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/sha512.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cache.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_cookie.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/ssl_ticket.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/threading.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/timing.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/version.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crl.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_crt.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/mbedtls/x509_csr.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/build_info.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_auto_enabled.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_dependencies.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_key_pair_types.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_adjust_config_synonyms.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_composites.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_key_derivation.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_builtin_primitives.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_compat.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_config.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_common.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_key_derivation.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_extra.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_legacy.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_platform.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_se_driver.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_sizes.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_struct.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_types.h"
    "/mnt/disk/App_linux/opt/esp-idf/components/mbedtls/mbedtls/include/psa/crypto_values.h"
    )
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "/home/sergio/CAP/firmware/build/esp-idf/mbedtls/mbedtls/include/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
